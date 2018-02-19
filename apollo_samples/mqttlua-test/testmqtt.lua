local args={
    host="你的Apollo所在服务器ip",
    port=61613,
    id="luaclient",
    topic_p="topic2",
    topic_s="topic1",
    debug=true
}
local running = true

function callback(
  topic,    -- string
  payload)  -- string
  --收到消息
  print("mqtt_test:callback(): " .. topic .. ": " .. payload)

  if (payload == "quit") then running = false end
end

local MQTT = require "mqtt"
if (args.debug) then MQTT.Utility.set_debug(true) end

local mqtt_client = MQTT.client.create(args.host, args.port, callback)

local connect_error = mqtt_client:connect(args.id)
while connect_error~=nil do
  connect_error = mqtt_client:connect(args.id)
end

mqtt_client:publish(args.topic_p, "Message from lua client")
mqtt_client:subscribe({ args.topic_s })

local error_message = nil
local running = true

while (error_message == nil and running) do
  error_message = mqtt_client:handler()

  if (error_message == nil) then
    socket.sleep(2.0)  -- seconds
  end
end

if (error_message == nil) then
  mqtt_client:unsubscribe({ args.topic_s })
  mqtt_client:destroy()
else
  print(error_message)
end

-- ------------------------------------------------------------------------- --
