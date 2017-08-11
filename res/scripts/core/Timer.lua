local Timer = Class("Timer")

function Timer:ctor(params)
    self.params = params OR {}
    self.updateHandle = params.updateHandle
    self.completeHandle = params.completeHandle
    self.duration = params.dt or 0
    self.elapse = self.duration
    self.params[#self.params + 1] = self.duration
    self.count = params.count or 0
    self.finish = false
end

function Timer:Update(dt)
    if self.finish then return true end

    self.elapse = self.elapse - dt
    if self.elapse <= 0 then 
        self.elapse = self.duration
        self:Apply()
    end
end

function Timer:Apply()
    if self.updateHandle then 
        self.updateHandle(table.unpack(self.params))
    end
    self.count = self.count - 1
    if self.count <= 0 then 
        self.count = 0
        self.finish = true
        if self.completeHandle then 
            self.completeHandle(table.unpack(self.params))
        end
    end
end

function Timer:Submit()
    return Engine.timer:Add(self)
end

return Timer