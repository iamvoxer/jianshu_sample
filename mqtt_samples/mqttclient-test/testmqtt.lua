local args={
    host="192.168.1.69",
    port=5555,
    id="client1",
    topic_p="test1",
    topic_s="test2",
    debug=true
}
local running = true

function callback(
  topic,    -- string
  payload)  -- string

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

mqtt_client:publish(args.topic_p, "*** Lua test start ***")
mqtt_client:subscribe({ args.topic_s })

local error_message = nil
local running = true

while (error_message == nil and running) do
  error_message = mqtt_client:handler()

  if (error_message == nil) then
    mqtt_client:publish(args.topic_p, "*** Lua test message ***")
    socket.sleep(1.0)  -- seconds
  end
end

if (error_message == nil) then
  mqtt_client:unsubscribe({ args.topic_s })
  mqtt_client:destroy()
else
  print(error_message)
end

-- ------------------------------------------------------------------------- --
