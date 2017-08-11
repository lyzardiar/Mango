local TimeOut = Class("TimeOut")

function TimeOut:ctor(dt, handle, ...)
    self.params = {...}
    self.handle = handle
    self.elapse = dt or 0
    self.finish = false
end

function TimeOut:Update(dt)
    if self.finish then return true end

    self.elapse = self.elapse - dt
    if self.elapse <= 0 then 
        self:Apply()
    end
end

function TimeOut:Apply()
    if self.handle then 
        self.handle(table.unpack(self.params))
    end
    self.finish = true
end

function TimeOut:Submit()
    return Engine.timer:Add(self)
end

return TimeOut