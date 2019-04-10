
local TimerOut = require("scripts.core.TimeOut")

local BaseComponent = Class("BaseComponent")


function BaseComponent:_define()
    self.Elapse = 0
    self.Duration = 2
    self.Bool = true
    self.String = "string"
end

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

    

    self:tick(dt)
    local ep = self:triggle()
    if ep then 
        -- print("Tick Update!!!", ep, self.Bool, self.String)
    end

    -- TimerOut:new(0, function()
    --     self.gameObject:RemoveComponent("scripts/core/BaseComponent.lua")
    -- end):Submit()    
end

function BaseComponent:OnDestroy()
    print("OnDestroy in BaseComponent:")
end

function BaseComponent:tick(dt)
    self.Elapse = self.Elapse + dt
end

function BaseComponent:triggle()
    if self.Elapse >= self.Duration then 
        local ep = self.Elapse
        self.Elapse = 0
        return ep
    end
    return false
end


return BaseComponent