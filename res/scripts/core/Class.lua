local objectCount = 0
local function getObjectCount()
    objectCount = objectCount + 1
    return objectCount
end

function Class(classname, super)
    local superType = type(super)
    local cls, set_funcs, get_funcs

    if super then
        cls = setmetatable({}, {__index = super, __call = super.__call})
        cls.super = super
    else    
        local __GSCall = function(handle, name)
            return function(call)
                if call then 
                    local func = handle
                    local changeHandle = function (self, val)
                        val = call(self, val) or val
                        local ret = func(self, val)
                        return ret
                    end
                    cls[name] = changeHandle
                end
            end
        end
        local __GSMT = { 
            get = function (name) 
                local handle, fname = Getter(cls, name) 
                return __GSCall(handle, fname)
            end, 
            set = function (name) 
                local handle, fname = Setter(cls, name) 
                return __GSCall(handle, fname)
            end, 
        }
        __GSMT.__index = __GSMT
        cls = setmetatable({ ctor = function() end, _define = function() end, }, __GSMT)
    end

    cls.__cname = classname

    local func       
    set_funcs = setmetatable({}, {
        __index = function(t, k)
            func = cls['set__' .. k]
            if func then 
                rawset(t, k, func)
            end
            return func
        end
    })
    get_funcs = setmetatable({}, {
        __index = function(t, k)
            func = cls['get__' .. k]
            if func then 
                rawset(t, k, func)
            end
            return func
        end
    })        
    cls.__newindex = function(o, k, v)
        func = set_funcs[k] --cls['set__' .. k]
        if func then 
            func(o, v)
        else 
            rawset(o, k, v) 
        end
    end
    cls.__index = function(o, k)
        func = get_funcs[k] --cls['get__' .. k]
        if func then return func(o, k) end

        local ret = cls[k]
        if ret then 
            rawset(o, k, ret)
            return ret
        end
        return rawget(o, k)
    end

    function cls:new(...)
        local instance = setmetatable({}, cls)
        instance.id = getObjectCount()
        instance.class = cls   

        local funcs = {}
        local super = cls 
        while super do 
            if super.super == nil or super._define ~= super.super._define then super._define(instance, ...) end
            if super.super == nil or super.ctor ~= super.super.ctor then table.insert(funcs, 1, super.ctor) end
            super = super.super
        end
        for i = 1, #funcs do 
            funcs[i](instance, ...)
        end
        return instance
    end

    function cls:create(...)
        return self:new(...)
    end

    function cls:isClass(classObj)
        local cls = self.class
        while cls do 
            if cls == classObj then return true end
            cls = cls.super
        end
        return false
    end 
    return cls
end

function Getter(cls, name)
    local func = function (self, val)
        --return val or self[name]
        return self[name]
    end
    local fname = 'get__' .. name:title()
    cls[fname] = func
    return func, fname
end

function Setter(cls, name)
    local func = function (self, val)
        self[name] = val
    end
    local fname = 'set__' .. name:title()
    cls[fname] = func
    return func, fname
end

function GSetter(cls, name)
    Setter(cls, name)
    Getter(cls, name)
end