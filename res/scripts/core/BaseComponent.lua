local BaseComponent = Class("BaseComponent")

function BaseComponent:ctor(gameObject)
    self.gameObject = gameObject
end

function BaseComponent:Awake()
    print("Awake in BaseComponent")
end

function BaseComponent:Start()
    print("Start in BaseComponent")
end

function BaseComponent:Update(dt)
    print("Update in BaseComponent:", dt)
end

return BaseComponent