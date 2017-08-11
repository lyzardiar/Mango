local TimerManager = Class("TimerManager")

function TimerManager:ctor()
    self.list = {}
end

function TimerManager:Add(tm)
    self.list[tm] = tm
    return tm
end

function TimerManager:Remove(tm)
    self.list[tm] = nil
end

function TimerManager:Update(dt)
    for k, tm in pairs(self.list) do 
        if tm:Update(dt) then 
            self.list[k] = nil
        end
    end
end



return TimerManager