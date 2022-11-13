#include "websocket.h"

#ifdef ENABLE_SERVER

AsyncWebSocket ws("/ws");

void sendStateAndInfo()
{
  DynamicJsonDocument jsonDocument(6144);
  for (int j = 0; j < ROWS * COLS; j++)
  {
    jsonDocument["data"][j] = Screen.getRenderBuffer()[j];
  }
  jsonDocument["mode"] = currentMode;
  jsonDocument["event"] = "info";
  jsonDocument["rotation"] = Screen.currentRotation;

  String output;
  serializeJson(jsonDocument, output);
  ws.textAll(output);
}

void sendModeToAllClients(MODE mode)
{
  DynamicJsonDocument jsonDocument(6144);

  jsonDocument["event"] = "mode";
  jsonDocument["mode"] = mode;
  jsonDocument["rotation"] = Screen.currentRotation;

  String output;
  serializeJson(jsonDocument, output);
  ws.textAll(output);
}

void onWsEvent(
    AsyncWebSocket *server,
    AsyncWebSocketClient *client,
    AwsEventType type,
    void *arg,
    uint8_t *data,
    size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    sendStateAndInfo();
  }

  if (type == WS_EVT_DATA)
  {
    AwsFrameInfo *info = (AwsFrameInfo *)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
    {
      data[len] = 0;

      DynamicJsonDocument wsRequest(6144);
      DeserializationError error = deserializeJson(wsRequest, data);

      if (error)
      {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }
      else
      {
        const char *event = wsRequest["event"];

        if (!strcmp(event, "mode"))
        {
          setModeByString(wsRequest["mode"], sendModeToAllClients);
        }
        else if (!strcmp(event, "rotate"))
        {
          bool isRight = (bool)!strcmp(wsRequest["direction"], "right");
          Screen.rotate(Screen.currentRotation + (90 * (isRight ? 1 : -1)));
          Screen.render();
        }
        else if (!strcmp(event, "info"))
        {
          sendStateAndInfo();
        }

        if (currentMode == NONE)
        {
          if (!strcmp(event, "clear"))
          {
            Screen.clear();
          }
          else if (!strcmp(event, "led"))
          {
            Screen.setPixelAtIndex(wsRequest["index"], wsRequest["status"]);
            Screen.render();
          }
          else if (!strcmp(event, "screen"))
          {
            uint8_t buffer[ROWS * COLS];
            for (int i = 0; i < ROWS * COLS; i++)
            {
              buffer[i] = wsRequest["data"][i];
            }
            Screen.setRenderBuffer(buffer);
            delay(10);
            Screen.render();
          }
          else if (!strcmp(event, "persist"))
          {
            Screen.persist();
          }
          else if (!strcmp(event, "load"))
          {
            Screen.loadFromStorage();
            delay(10);
            Screen.render();
            sendStateAndInfo();
          }
        }
      }
    }
  }
}

void initWebsocketServer(AsyncWebServer &server)
{
  server.addHandler(&ws);
  ws.onEvent(onWsEvent);
}

void cleanUpClients()
{
  ws.cleanupClients();
}

#endif
