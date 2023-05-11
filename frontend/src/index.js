const count = 16;
const size = 16;
const spacing = 1;
const width = count * (size + spacing) - spacing;

import examples from './examples.json';

const input = document.getElementById('input');
const editor = document.getElementById('editor');
const comment = document.getElementById('comment');
const output = document.getElementById('output');
const context = output.getContext('2d');
const dpr = window.devicePixelRatio || 1;

let callback = function () { };
let startTime = null;
let code = '';

output.width = output.height = width * dpr;
context.scale(dpr, dpr);
output.style.width = output.style.height = `${width}px`;

////////// Connect to the IKEA OBEGRÄNSAD Hack/Mod ESP32 web socket /////////
let wsUrl = `ws://${window.location.host}/ws`;

let ws = new WebSocket(wsUrl);

ws.onopen = () => {
  console.log('WebSocket connection opened');
};

ws.onerror = (error) => {
  console.log(`WebSocket error: ${error}`);
};

ws.onmessage = (message) => {
  console.log(`Received: ${message.data}`);
};

let ledData = Array(256).fill(0); // Initialize array of intensity values (0-1)

///////////////////////////////////

function readURL() {
  const url = new URL(document.location);

  if (url.searchParams.has('code')) {
    input.value = url.searchParams.get('code');
  }
}

readURL();

function checkLength() {
  if (code.length > 32) {
    editor.classList.add('over-limit');
  } else {
    editor.classList.remove('over-limit');
  }
}

function updateCallback() {
  code = input.value;
  startTime = null;

  checkLength();

  try {
    callback = new Function('t', 'i', 'x', 'y', `
      try {
        with (Math) {
          return ${code};
        }
      } catch (error) {
        return error;
      }
    `);
  } catch (error) {
    callback = null;
  }
}

input.addEventListener('input', updateCallback);
updateCallback();

input.addEventListener('focus', function () {
  editor.classList.add('focus');
  updateComments([
    'hit "enter" to save in URL',
    'or get <a href="https://twitter.com/aemkei/status/1323399877611708416">more info here</a>'
  ]);
});

input.addEventListener('blur', function () {
  updateCommentsForCode();
  editor.classList.remove('focus');
});

editor.addEventListener('submit', (event) => {
  event.preventDefault();
  const url = new URL(document.location);
  url.searchParams.set('code', code);
  history.replaceState(null, code, url);
});


function render() {
  let time = 0;

  if (startTime) {
    time = (new Date() - startTime) / 1000;
  } else {
    startTime = new Date();
  }

  if (!callback) {
    window.requestAnimationFrame(render);
    return;
  }

  output.width = output.height = width * dpr;
  context.scale(dpr, dpr);
  let index = 0;

  for (let y = 0; y < count; y++) {
    for (let x = 0; x < count; x++) {
      let value = Number(callback(time, index, x, y));

      value = value < 0 ? 0 : value;
      value = value > 1 ? 1 : value;

      const offset = size / 2;
      let radius = (value * size) / 2;

      if (radius < 0) {
        radius = 0;
      }

      if (radius > size / 2) {
        radius = size / 2;
      }

      context.beginPath();
      context.fillStyle = '#FFF';
      context.arc(
        x * (size + spacing) + offset,
        y * (size + spacing) + offset,
        radius,
        0,
        2 * Math.PI
      );
      context.fill();

      // Store the value between 0 and 1 to use as intensity 
      ledData[index] = value; // index should just be y*count+x
      index++;
    }
  }

  window.requestAnimationFrame(render);
}

render();

////// Managing Led State ///////////////////

let ledState = new Array(256).fill(0); // Array to store the actual on/off state of each LED (0 or 1)

const updateLedStatePeriod = 50;
const sendPeriod = 5000;

function updateLedState() {

  for (let i = 0; i < ledData.length; i++) {

    ledState[i] = ledData[i] < 0.5 ? 0 : 1;
  }
}

function sendLedData() {
  console.log("sending: ", ledState)

  if (ws.readyState === WebSocket.OPEN) {
    ws.send(JSON.stringify({ event: "screen", data: ledState }));
  }

}

setInterval(updateLedState, updateLedStatePeriod);
setInterval(sendLedData, sendPeriod);

////////////////////////////////////////////////////////////////////////////

function updateComments(comments) {
  const lines = comment.querySelectorAll('label');

  if (comments.length === 1) {
    lines[0].innerHTML = '&nbsp;';
    lines[1].innerHTML = `// ${comments[0]}`;
  } else {
    lines[0].innerHTML = `// ${comments[0]}`;
    lines[1].innerHTML = `// ${comments[1]}`;
  }
}

function updateCommentsForCode() {
  const code = input.value;

  const snippets = Object.values(examples);
  const comments = Object.keys(examples);
  const index = snippets.indexOf(code);
  const newComment = comments[index];

  if (!newComment) {
    return;
  }

  const newComments = newComment.split('\n');

  updateComments(newComments);
}

function nextExample() {
  const snippets = Object.values(examples);

  let index = snippets.indexOf(code);

  if (snippets[index + 1]) {
    index = index + 1;
  } else {
    return;
  }

  const newCode = snippets[index];
  input.value = newCode;

  updateCommentsForCode();

  // history.replaceState({
  //   code: newCode,
  //   comment: newComment
  // }, code, `?code=${encodeURIComponent(newCode)}`);

  updateCallback();
}

output.addEventListener('click', nextExample);

window.onpopstate = function (event) {
  readURL();
  updateCallback();
};

updateCommentsForCode();
