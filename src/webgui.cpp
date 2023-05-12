#include "webgui.h"

#ifdef ENABLE_SERVER

#include <ESPAsyncWebServer.h>

const uint32_t GUI_HTML_SIZE = 3621;
const uint8_t GUI_HTML[] PROGMEM = {31,139,8,0,0,0,0,0,2,3,133,121,105,123,218,56,247,247,251,126,10,213,157,166,54,24,131,201,210,20,
112,58,93,175,233,222,107,246,185,50,121,110,132,125,0,77,140,228,74,50,224,166,124,173,251,213,253,106,62,217,
115,142,140,41,73,151,127,155,4,45,231,119,118,29,45,140,110,103,42,181,85,1,108,110,23,249,217,173,81,253,
129,159,192,179,179,91,140,141,22,96,57,147,124,1,137,183,20,176,42,148,182,30,75,149,180,32,109,226,173,68,
102,231,201,81,175,23,178,210,128,238,152,148,231,124,146,67,34,149,199,186,55,25,216,57,44,160,147,170,92,233,
61,30,119,122,238,95,67,111,133,205,225,204,183,161,8,215,97,21,176,228,140,121,169,6,110,197,18,16,148,1,
155,169,124,42,228,204,27,117,29,237,77,41,25,152,84,139,194,10,37,247,164,60,98,11,33,197,130,231,194,88,
98,131,12,24,200,165,208,74,46,144,32,98,79,144,82,171,156,197,39,235,248,132,21,74,72,107,216,74,216,57,
227,204,32,117,14,236,37,95,242,95,28,111,54,45,101,74,18,34,143,196,223,34,21,76,61,67,222,76,188,133,
202,202,28,80,190,86,198,40,45,102,66,158,221,242,27,148,31,92,161,98,168,136,76,208,255,165,83,192,217,8,
207,114,160,158,239,229,66,94,122,65,164,33,127,141,26,15,197,212,151,7,7,50,50,101,65,33,48,251,109,127,
43,173,64,98,197,51,47,8,52,216,82,203,225,84,105,191,150,99,153,154,178,157,168,15,37,232,234,23,200,33,
181,74,63,202,115,255,30,73,59,71,120,114,131,213,197,189,32,48,190,13,134,18,86,236,77,105,57,105,255,110,
98,64,47,65,251,54,57,187,250,44,67,145,12,27,160,166,42,114,78,72,18,47,157,139,60,35,3,188,224,51,
225,138,8,85,196,179,12,178,183,10,195,21,172,34,203,103,111,249,194,97,94,191,120,251,202,59,56,88,145,237,
216,191,174,17,78,24,127,21,108,130,72,213,90,248,141,85,225,213,78,216,224,118,47,52,229,196,106,0,108,110,
130,97,227,119,38,124,219,184,94,37,87,155,97,237,40,102,35,140,54,204,180,176,213,193,129,175,62,247,146,189,
153,32,180,168,210,20,180,6,253,94,229,34,117,180,55,134,146,155,52,132,114,73,240,206,37,1,25,84,26,232,
96,180,51,84,90,240,220,120,15,85,180,215,77,60,33,211,188,204,192,27,124,129,228,82,201,106,161,202,47,49,
106,33,172,55,184,49,104,208,163,157,58,249,188,80,109,118,94,48,228,5,140,147,141,160,104,114,133,218,201,237,
222,176,113,142,160,168,79,193,166,115,36,155,163,77,161,10,54,155,192,15,182,20,31,80,207,187,71,172,195,170,
187,71,94,248,40,241,214,216,59,56,168,187,79,176,123,118,200,176,231,254,174,71,113,159,58,248,225,133,207,19,
239,136,181,152,197,1,65,115,52,225,133,63,160,182,66,250,235,110,63,64,158,174,217,177,212,172,218,39,94,248,
34,241,42,150,36,72,252,233,19,235,248,241,113,103,77,221,42,240,194,223,80,84,146,244,216,39,134,31,241,49,
126,86,117,183,162,174,23,190,77,188,211,150,189,27,31,34,175,121,85,40,139,140,239,71,199,33,171,232,3,25,
252,92,75,238,183,184,229,210,247,235,225,110,77,21,4,237,227,150,69,162,95,19,15,71,170,70,55,27,160,5,
49,42,246,79,13,182,173,99,26,177,156,154,247,145,126,146,92,121,152,240,12,150,160,43,150,41,203,182,153,214,
99,56,26,179,191,165,85,44,157,115,57,3,102,231,192,150,194,136,137,200,49,203,188,129,247,134,219,121,164,185,
204,212,194,15,216,136,245,162,216,11,41,107,24,103,211,92,113,203,38,96,87,0,196,13,169,88,252,183,100,200,
46,3,172,138,53,59,35,62,194,77,70,200,162,224,26,51,194,130,102,99,59,102,194,144,26,72,109,197,2,152,
144,204,0,134,54,51,13,176,54,180,129,177,177,112,16,2,8,153,193,26,193,106,74,93,103,158,223,139,162,254,
241,113,128,96,193,186,172,127,124,130,192,241,122,7,193,162,95,46,100,141,68,117,167,90,45,88,143,180,142,143,
17,178,70,72,236,16,213,14,161,213,10,9,49,143,213,77,234,170,161,46,148,17,110,91,144,229,98,2,218,48,
174,129,173,230,194,66,248,183,148,48,227,52,89,143,106,200,28,178,195,48,172,91,111,54,182,187,96,112,218,28,
44,70,130,231,37,152,45,173,69,202,69,153,91,81,228,213,142,252,102,236,76,1,59,230,182,117,132,144,186,148,
179,130,91,11,90,26,86,210,254,129,254,202,196,116,10,26,164,101,110,15,68,204,121,28,178,94,200,58,241,197,
185,184,123,120,129,88,115,41,10,54,118,1,24,147,197,164,40,198,108,174,50,10,23,133,187,208,170,48,44,23,
151,192,198,200,120,76,25,53,126,255,98,140,236,92,200,58,230,131,182,254,54,129,91,173,126,27,115,250,132,26,
65,64,230,40,13,12,214,124,81,228,96,88,20,69,91,88,213,61,101,109,70,241,198,46,77,50,171,5,167,109,
111,107,218,26,103,62,148,92,163,73,251,83,126,197,206,216,58,160,165,239,199,71,184,46,71,110,89,110,109,31,
124,8,177,118,102,131,71,161,33,44,12,158,132,222,214,211,25,171,135,144,71,199,95,159,89,87,45,108,93,45,
142,59,174,235,26,181,210,50,35,145,53,196,133,199,25,197,90,12,237,60,169,73,150,228,100,87,48,235,233,163,
150,253,212,219,146,244,59,216,70,50,35,64,23,66,162,147,7,207,67,111,34,36,215,21,75,115,149,94,18,198,
182,98,4,144,33,163,81,109,18,22,145,83,100,94,175,33,38,149,48,164,111,221,245,137,119,31,61,19,147,207,
104,107,76,153,89,40,101,231,13,161,115,172,96,45,36,11,188,125,195,175,83,165,202,248,22,157,47,240,119,205,
90,206,127,43,142,153,59,248,1,117,204,149,42,216,222,223,191,229,164,98,63,46,251,49,34,209,178,211,160,133,
182,158,126,174,76,173,19,74,193,169,208,176,82,250,210,212,228,197,127,232,228,35,93,185,248,145,195,226,18,4,
249,61,58,234,250,77,81,196,26,217,11,43,252,56,13,240,79,191,245,128,84,214,162,160,60,169,153,96,178,155,
66,88,184,86,32,58,117,147,82,110,221,90,183,171,86,229,210,204,96,32,242,156,34,242,235,139,63,255,98,83,
37,109,205,132,91,48,51,165,121,78,217,127,124,116,114,24,246,227,7,71,97,255,240,244,228,226,188,106,219,214,
131,131,251,23,7,20,128,14,249,245,48,195,165,6,233,37,104,54,81,92,103,215,204,33,15,248,206,7,93,66,
30,7,7,241,255,139,187,85,235,244,32,14,90,85,247,216,197,69,164,151,149,243,220,22,251,143,130,202,174,68,
150,213,153,221,177,173,195,246,131,246,97,139,162,176,110,29,98,194,117,142,91,110,3,194,74,94,107,128,193,213,
83,1,249,23,210,179,4,237,189,123,28,61,104,199,225,109,212,132,148,232,117,51,212,3,183,144,12,193,153,224,
185,154,149,176,61,72,74,134,64,144,53,155,116,46,10,91,74,210,34,238,30,246,91,110,247,232,158,28,181,214,
174,41,58,235,218,147,5,79,169,228,46,121,6,186,70,94,2,114,75,115,174,47,83,85,94,98,214,252,248,81,
125,44,57,21,148,123,197,230,223,255,254,251,191,209,191,255,251,247,191,155,226,94,132,117,74,63,81,25,60,194,
240,4,7,253,86,171,66,150,243,82,206,48,239,145,49,91,112,249,189,12,105,146,35,177,119,143,90,199,152,30,
201,105,48,58,57,56,88,143,170,79,213,168,179,118,250,65,106,117,185,64,36,207,43,3,250,107,110,94,31,84,
163,7,7,213,217,81,155,60,139,27,114,27,147,37,104,175,187,152,171,232,163,153,66,172,25,188,8,167,180,65,
13,126,11,51,172,113,131,183,161,86,245,137,115,240,115,184,18,5,12,126,69,113,106,106,25,117,234,244,175,130,
110,255,104,151,251,196,204,164,106,240,79,232,9,89,148,150,9,42,148,11,97,33,171,183,220,195,62,35,143,240,
212,130,54,183,107,22,174,72,162,19,253,170,110,34,183,7,15,90,13,67,47,205,49,131,216,28,52,48,171,144,
203,182,184,87,170,212,76,173,36,249,252,167,71,191,63,99,207,127,123,123,251,158,183,9,151,73,124,18,150,244,
103,158,156,132,111,146,24,15,27,69,178,108,249,101,123,30,116,230,97,254,249,200,63,3,187,61,239,63,174,94,
100,126,173,179,23,132,213,183,73,32,19,86,105,164,121,250,109,154,84,45,168,131,68,250,219,68,170,180,181,48,
158,104,154,164,235,15,172,173,239,245,51,28,76,147,149,144,153,90,69,25,44,69,10,239,197,26,242,159,41,20,
159,62,197,195,28,44,91,39,123,119,153,77,56,77,100,153,231,97,150,120,222,80,71,245,173,176,104,165,216,158,
131,152,205,45,117,90,111,134,60,162,11,34,248,105,152,182,208,47,1,206,27,91,229,176,69,140,127,184,42,54,
197,122,220,12,55,96,26,111,189,161,25,39,251,167,100,188,50,131,110,247,135,171,173,146,88,191,93,154,68,115,
101,236,166,187,50,227,112,145,208,141,229,15,152,252,130,181,29,172,255,83,48,92,68,74,170,2,100,226,7,201,
153,187,4,168,28,16,59,243,189,29,29,195,97,9,206,46,70,180,128,206,216,56,36,104,173,116,2,55,144,227,
207,72,71,48,96,63,92,193,102,188,197,44,192,24,62,131,47,81,63,67,10,98,9,153,35,143,50,110,185,195,
144,113,175,147,71,90,243,202,199,19,84,16,77,69,158,251,189,32,188,116,214,124,49,209,156,197,63,38,199,159,
175,56,143,253,192,93,202,136,25,36,189,33,140,94,71,57,200,153,157,15,161,221,14,46,207,225,34,169,15,134,
170,148,153,255,26,251,45,58,185,109,118,28,254,240,221,237,224,154,135,12,72,186,50,15,152,23,94,6,225,34,
210,192,179,234,23,203,45,93,217,118,78,136,222,189,127,246,54,184,170,69,147,202,191,9,105,79,159,228,124,81,
64,86,171,127,73,129,32,110,62,68,147,146,206,67,120,173,48,96,95,72,11,122,201,115,255,143,240,227,222,133,
237,125,115,85,110,24,254,252,122,119,227,219,197,61,24,2,178,228,58,157,191,167,163,170,137,230,220,208,50,200,
192,11,240,142,150,71,238,80,151,220,32,194,164,111,136,130,13,202,249,44,244,63,40,52,219,58,237,236,176,255,
176,138,176,216,26,67,87,203,136,103,180,120,150,160,59,174,176,120,193,96,127,86,195,2,231,174,17,236,57,118,
70,124,147,173,58,205,154,65,97,67,171,171,171,181,179,239,121,179,170,60,58,123,10,252,165,26,139,69,123,124,
139,185,127,72,202,174,182,109,86,111,43,62,133,51,216,27,101,219,219,6,102,87,182,25,238,134,55,219,214,134,
161,219,82,196,185,148,37,224,13,152,27,111,112,53,10,211,211,129,72,77,212,122,179,201,201,19,207,150,32,45,
25,14,18,116,83,191,194,89,48,68,75,135,95,163,152,98,232,140,23,238,213,142,47,124,91,147,4,225,47,254,
249,189,185,176,204,3,202,12,143,138,175,225,75,218,9,41,9,238,133,247,148,102,24,66,54,226,140,174,167,137,
55,183,182,160,178,96,209,41,136,136,176,12,118,235,141,172,75,167,179,210,116,227,195,254,225,225,131,7,167,247,
239,159,196,241,253,222,233,17,222,34,206,22,74,19,211,169,114,53,126,212,229,103,247,46,130,205,215,213,159,228,
165,190,166,253,59,63,8,191,140,127,99,4,177,169,190,194,198,148,19,202,140,144,234,2,68,133,6,154,126,10,
83,94,230,118,119,193,150,223,203,119,121,61,149,205,46,149,195,44,8,231,40,72,233,10,149,41,114,158,130,91,
165,126,93,159,67,25,236,63,135,60,67,19,154,58,129,43,126,250,16,18,159,164,62,69,68,103,26,116,99,56,
28,76,147,102,36,188,189,14,154,146,171,1,31,145,140,125,36,197,194,169,244,28,245,0,255,89,176,125,81,217,
52,155,192,254,6,16,238,149,255,192,21,59,137,114,155,90,37,72,135,209,114,40,176,72,53,99,6,199,12,142,
25,28,115,138,218,228,173,187,234,249,107,31,66,25,154,80,4,193,208,38,118,212,123,216,27,216,16,91,103,241,
195,120,96,155,103,12,196,243,104,2,51,33,223,115,59,199,96,113,87,58,127,161,221,5,95,30,159,63,127,238,
225,208,44,87,19,158,63,202,139,57,79,44,246,53,164,214,55,245,142,221,86,161,216,181,202,176,236,190,105,152,
32,183,215,231,242,2,17,178,221,222,60,198,238,255,225,155,205,179,253,34,243,139,15,159,95,1,159,126,249,38,
231,229,124,2,185,71,181,173,174,68,88,104,227,135,190,60,239,93,68,66,74,208,63,253,250,230,117,226,29,200,
137,41,134,94,40,207,227,253,241,113,183,75,123,11,18,227,206,50,32,212,183,102,191,133,140,113,110,175,118,189,
219,43,196,77,5,147,201,187,201,63,168,112,221,53,254,36,8,69,51,116,9,149,27,48,137,140,220,173,255,221,
20,13,198,8,137,115,115,65,201,118,219,54,79,79,77,172,108,100,138,92,88,127,124,107,28,12,127,241,213,158,
244,87,123,210,111,200,108,50,9,118,98,178,128,248,195,185,108,199,23,129,76,240,99,8,185,1,118,77,156,72,
0,131,55,108,246,6,17,210,82,198,178,181,209,95,46,216,250,12,232,133,175,130,225,54,196,74,22,170,48,110,
251,107,116,164,112,190,223,242,24,34,179,33,190,162,119,235,55,97,247,66,109,108,69,111,213,244,192,126,229,174,
255,131,59,211,233,116,88,47,143,65,220,235,221,29,78,120,122,57,115,219,114,167,166,160,167,241,77,235,106,162,
214,29,35,62,210,254,59,81,58,3,221,193,145,13,31,208,171,109,200,7,244,120,100,33,219,113,237,31,109,72,
74,216,10,93,65,190,162,203,87,103,202,23,34,175,6,11,37,149,187,81,12,105,148,184,194,32,62,42,214,155,
137,202,170,171,125,101,92,237,232,32,227,133,25,164,174,6,15,23,92,227,74,26,244,134,5,186,8,181,193,86,
38,12,18,86,3,186,229,111,92,198,214,66,195,59,245,1,243,170,33,152,208,37,123,115,167,62,191,94,185,226,
48,232,223,143,139,245,16,173,128,78,35,57,138,143,97,49,252,42,136,221,113,156,247,157,231,216,220,244,221,160,
199,122,195,218,79,131,94,163,179,227,203,246,52,191,193,117,224,42,246,21,42,77,234,12,164,146,176,163,112,102,
69,110,55,239,172,184,150,136,111,204,186,78,120,103,123,242,222,139,68,51,23,125,62,17,124,105,199,190,242,223,
0,69,78,191,239,169,210,56,235,251,208,250,141,47,68,77,51,232,232,146,86,253,117,91,40,107,235,84,197,134,
251,22,8,71,40,57,92,18,167,92,46,185,97,34,75,154,11,196,217,168,91,15,34,29,18,96,229,94,208,116,
115,81,33,20,14,103,98,233,70,183,14,170,135,105,194,105,197,220,6,154,208,163,133,177,30,67,30,244,8,238,
184,99,49,178,98,77,239,79,95,253,14,104,212,117,12,190,206,174,182,245,11,126,110,49,55,15,151,134,102,217,
238,12,240,93,126,215,220,254,5,219,210,192,245,235,37,163,212,1,99,174,241,28,117,209,21,232,170,61,238,228,
23,215,186,206,114,255,27,176,235,44,28,65,253,101,87,189,233,35,139,6,182,253,250,201,194,26,219,117,105,107,
30,107,220,27,95,219,6,30,227,165,85,24,137,34,7,139,211,106,58,221,14,241,66,88,158,139,143,205,160,41,
32,207,221,243,11,198,134,231,6,26,168,166,29,114,75,228,74,3,86,251,185,144,56,52,219,125,239,215,165,84,
112,150,98,34,213,9,132,13,247,253,226,255,7,120,243,78,230,118,28,0,0};

void startGui(AsyncWebServerRequest *request)
{
  AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", GUI_HTML, GUI_HTML_SIZE);
  response->addHeader("Content-Encoding", "gzip");
  request->send(response);
}

#endif
