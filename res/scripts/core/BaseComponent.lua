
local TimerOut = require("scripts.core.TimeOut")

local BaseComponent = Class("BaseComponent")

function BaseComponent:ctor(gameObject)
    self.gameObject = gameObject

    print(self, BaseComponent, gameObject)
end

function BaseComponent:Awake()
    print("Awake in BaseComponent")
end

function BaseComponent:Start()
    print("Start in BaseComponent")
end

function BaseComponent:Update(dt)
    --print("Update in BaseComponent:", dt)

    TimerOut:new(0, function()
        self.gameObject:RemoveComponent("scripts/core/BaseComponent.lua")
    end):Submit()    
end

function BaseComponent:OnDestroy()
    print("OnDestroy in BaseComponent:")
end

return BaseComponent