require("scripts.core.Class")

local flag = 1

function Engine.Update(dt)
    -- print("Now Engine Update...", dt)

    if flag == 1 then 
        flag = 2
        Engine.instance.root:AddComponent("scripts/core/BaseComponent.lua")
    end
end