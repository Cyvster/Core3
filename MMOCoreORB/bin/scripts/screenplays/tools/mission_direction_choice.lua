-- Allows players to choose the direction they want to take destroy missions
--
--							N 0 and 360
--							|
--					NW 45   |	NE 315
--						  \ | /
--						   \|/
--			W 90  ----------+---------- E 270
--						   /|\
--						  / | \
--					SW 135	|  SE 225
--							|
--							S 180

mission_direction_choice = ScreenPlay:new {
	numberOfActs = 1,

	directions = {
		{dirDesc = "Reset mission direction", dirSelect = -1},
		{dirDesc = "North", dirSelect = 0},
		{dirDesc = "North East", dirSelect = 315},
		{dirDesc = "East", dirSelect = 270}, 
		{dirDesc = "South East", dirSelect = 225}, 
		{dirDesc = "South", dirSelect = 180}, 
		{dirDesc = "South West", dirSelect = 135}, 
		{dirDesc = "West", dirSelect = 90}, 
		{dirDesc = "North West", dirSelect = 45}, 
	}
}

function mission_direction_choice:start()

end

function mission_direction_choice:openWindow(pPlayer)
	if (pPlayer == nil) then
		return
	end

	self:showLevels(pPlayer)
end

function mission_direction_choice:showLevels(pPlayer)

	local cancelPressed = (eventIndex == 1)

	if (cancelPressed) then
		return
	end

	local sui = SuiListBox.new("mission_direction_choice", "dirSelection") -- calls dirSelection on SUI window event

	sui.setTargetNetworkId(SceneObject(pPlayer):getObjectID())

	sui.setTitle("Mission Direction Selection")

	local promptText = "Select a direction which you would like to take missions.  After you have chosen, use the mission terminal to get a selection of missions (if any exist) in that direction.  \n\nIf no missions are offered to you, it is because terrain is unsuitable for missions in that direction from your current location and you will need to choose another direction.\n\nWhen you want to go back to the 'normal' selection of missions (any direction), just choose Reset mission direction."

	sui.setPrompt(promptText)

	for i = 1,  #self.directions, 1 do
		sui.add(self.directions[i].dirDesc, "")
	end

	sui.sendTo(pPlayer)
end

function  mission_direction_choice:dirSelection(pPlayer, pSui, eventIndex, args)

	local cancelPressed = (eventIndex == 1)

	if (cancelPressed) then
		return 
	end

	if (args == "-1") then
		CreatureObject(pPlayer):sendSystemMessage("No direction was selected...")
		return
	end

	local selectedIndex = tonumber(args)+1

	local selectedDir = tonumber(self.directions[selectedIndex].dirSelect)
	local selectedDirDesc = self.directions[selectedIndex].dirDesc

	writeScreenPlayData(pPlayer, "mission_direction_choice", "directionChoice", selectedDir) 

	if (selectedDir == 0) then
		CreatureObject(pPlayer):sendSystemMessage("Mission direction has been reset to normal.")
	else	
		CreatureObject(pPlayer):sendSystemMessage("You have selected missions to the " .. selectedDirDesc .. ". This choice will remain active until you change or reset it.")
	end

end