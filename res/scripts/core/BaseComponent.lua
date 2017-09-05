
local TimerOut = require("scripts.core.TimeOut")

local BaseComponent = Class("BaseComponent")


function BaseComponent:_define()
    self._elapse = 0
    self._duration = 2
    self._bool = true
    self._string = "string"
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

    self.totcnt = 30
    self.tottime = self.tottime or 0
    self.tottick = self.tottick or 0

    self.tottime = self.tottime + dt
    self.tottick = self.tottick + 1
    if self.tottick >= self.totcnt then 
        print("Frame DT:", self.tottime / self.totcnt)
        self.tottime = 0
        self.tottick = 0
    end

    self:tick(dt)
    local ep = self:triggle()
    if ep then 
        print("Tick Update!!!", ep, self._bool, self._string)
    end

    -- TimerOut:new(0, function()
    --     self.gameObject:RemoveComponent("scripts/core/BaseComponent.lua")
    -- end):Submit()    
end

function BaseComponent:OnDestroy()
    print("OnDestroy in BaseComponent:")
end

function BaseComponent:tick(dt)
    self._elapse = self._elapse + dt
end

function BaseComponent:triggle()
    if self._elapse >= self._duration then 
        local ep = self._elapse
        self._elapse = 0
        return ep
    end
    return false
end


return BaseComponent