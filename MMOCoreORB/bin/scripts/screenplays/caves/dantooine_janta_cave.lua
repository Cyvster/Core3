JantaCaveScreenPlay = ScreenPlay:new {
	numberOfActs = 1,

	screenplayName = "JantaCaveScreenPlay",

	lootContainers = {
		529346,
        	529349,
        	529353,
        	529363,
        	529368
	},
	
	lootLevel = 26,	

	lootGroups = {
		{
			groups = {
				{group = "color_crystals", chance = 160000},
				{group = "junk", chance = 8240000},
				{group = "rifles", chance = 500000},
				{group = "pistols", chance = 500000},
				{group = "clothing_attachments", chance = 300000},
				{group = "armor_attachments", chance = 300000}
			},
			lootChance = 8000000
		}					
	},
	
	lootContainerRespawn = 1800 
}

registerScreenPlay("JantaCaveScreenPlay", true)

function JantaCaveScreenPlay:start()
	if (isZoneEnabled("dantooine")) then
		self:spawnMobiles()
		self:initializeLootContainers()
	end
end

function JantaCaveScreenPlay:spawnMobiles()
	spawnMobile("dantooine", "janta_clan_leader",180,-91.6,-100.4,-93.6,-178,529333)

	spawnMobile("dantooine", "janta_rockshaper",180,6.9,-40.8,-73.5,-3,529325)
	spawnMobile("dantooine", "janta_rockshaper",180,-5.5,-64.9,-240.8,-83,529329)
	spawnMobile("dantooine", "janta_rockshaper",180,-94.6,-73.7,-163.4,-174,529330)
	spawnMobile("dantooine", "janta_rockshaper",180,-98.9,-73.3,-165.8,130,529330)

	spawnMobile("dantooine", "janta_rockshaper",180,-9,-40.5,-71.6,77,529325)
	spawnMobile("dantooine", "janta_rockshaper",180,-3.5,-40.5,-75.2,5,529325)
	spawnMobile("dantooine", "janta_rockshaper",180,-96.6,-102.4,-138.3,103,529330)
	spawnMobile("dantooine", "janta_rockshaper",180,-87.6,-101.9,-136.5,163,529330)

	spawnMobile("dantooine", "janta_rockshaper",180,65.7,-54.5,-152.7,-101,529327)
	spawnMobile("dantooine", "janta_rockshaper",180,-5.4,-64.2,-234.8,-43,529329)
	spawnMobile("dantooine", "janta_rockshaper",180,-97.6,-72.7,-174.4,71,529330)
	spawnMobile("dantooine", "janta_rockshaper",180,-32.3,-70.8,-85.9,-168,529331)

	spawnMobile("dantooine", "janta_rockshaper",180,49.9,-46.2,-62.6,-99,529326)
	spawnMobile("dantooine", "janta_rockshaper",180,46.4,-46.3,-60.5,-105,529326)
	spawnMobile("dantooine", "janta_rockshaper",180,41.9,-46.1,-98,-22,529326)
	spawnMobile("dantooine", "janta_rockshaper",180,-28.7,-80.1,-148.5,11,529332)

	spawnMobile("dantooine", "janta_rockshaper",180,45.4,-46.2,-93.1,-21,529326)
	spawnMobile("dantooine", "janta_rockshaper",180,58.3,-46.9,-123.3,-103,529327)
	spawnMobile("dantooine", "janta_rockshaper",180,58.5,-47.8,-127,-91,529327)
	spawnMobile("dantooine", "janta_rockshaper",180,-68.8,-70.2,-88.8,63,529331)

	spawnMobile("dantooine", "janta_rockshaper",180,-6.6,-45.4,-145.1,22,529327)
	spawnMobile("dantooine", "janta_rockshaper",180,-14.9,-63.7,-259.9,1,529329)
	spawnMobile("dantooine", "janta_rockshaper",180,-9.7,-63.8,-260.6,-9,529329)
	spawnMobile("dantooine", "janta_rockshaper",180,-21.5,-79.5,-143.4,-32,529332)

	spawnMobile("dantooine", "janta_rockshaper",180,-0.9,-45,-141.7,18,529327)
	spawnMobile("dantooine", "janta_rockshaper",180,-4.8,-65.5,-209.6,-94,529329)
	spawnMobile("dantooine", "janta_rockshaper",180,-94.5,-101.9,-143.1,87,529330)
	spawnMobile("dantooine", "janta_rockshaper",180,-113.9,-69.4,-124.1,88,529330)

	spawnMobile("dantooine", "janta_rockshaper",180,-59.6,-70.9,-180.4,-95,529330)
	spawnMobile("dantooine", "janta_rockshaper",180,-63.8,-69.9,-171.1,-131,529330)
	spawnMobile("dantooine", "janta_rockshaper",180,-26.1,-70.9,-148.2,-46,529332)

	spawnMobile("dantooine", "janta_rockshaper",180,-10.9,-64,-183.5,79,529328)
	spawnMobile("dantooine", "janta_rockshaper",180,-0.8,-65.7,-211.8,-91,529329)
	spawnMobile("dantooine", "janta_rockshaper",180,-29.2,-70.4,-87.1,166,529331)
	spawnMobile("dantooine", "janta_rockshaper",180,-25.4,-69.7,-91.3,-55,529331)

	spawnMobile("dantooine", "janta_rockshaper",180,11,-40.9,-69.7,-58,529325)
	spawnMobile("dantooine", "janta_rockshaper",180,60.3,-55.7,-154.6,72,529327)
	spawnMobile("dantooine", "janta_rockshaper",180,-8.2,-64,-229.5,-76,529329)
	spawnMobile("dantooine", "janta_rockshaper",180,-62.2,-70.3,-87.2,-136,529331)

	spawnMobile("dantooine", "janta_rockshaper",180,67,-54.8,-156.2,-85,529327)
	spawnMobile("dantooine", "janta_rockshaper",180,-93,-70.4,-123.2,-96,529330)
	spawnMobile("dantooine", "janta_rockshaper",180,-84.9,-100.1,-97.2,-109,529333)
	spawnMobile("dantooine", "janta_rockshaper",180,-98,-100.5,-100.5,79,529333)

	--cyvster's solo server custom spawns
	spawnMobile("dantooine", "janta_rockshaper",180,-19.1,-65.5,-211.1,114,529329)
	spawnMobile("dantooine", "janta_rockshaper",180,-19.2,-65.0,-218.1,70,529329)
	spawnMobile("dantooine", "janta_rockshaper",180,-15.6,-64.1,-223.6,-87,529329)
	spawnMobile("dantooine", "janta_rockshaper",180,-2.0,-64.9,-208.0,-120,529329)
	spawnMobile("dantooine", "janta_rockshaper",180,-5.6,-66.2,-212.6,-93,529329)
	spawnMobile("dantooine", "janta_rockshaper",180,-1.6,-63.9,-231.3,-67,529329)
	spawnMobile("dantooine", "janta_rockshaper",180,-2.2,-64.7,-240.3,-157,529329)
	spawnMobile("dantooine", "janta_rockshaper",180,-17.4,-65.6,-240.1,-59,529329)
	spawnMobile("dantooine", "janta_rockshaper",180,-23.8,-65.2,-248.6,76,529329)
	spawnMobile("dantooine", "janta_rockshaper",180,-21.0,-64.2,-254.8,55,529329)
	spawnMobile("dantooine", "janta_rockshaper",180,21.5,-61.9,-171.0,94,529327)
	spawnMobile("dantooine", "janta_rockshaper",180,25.6,-60.3,-169.4,139,529327)
	spawnMobile("dantooine", "janta_rockshaper",180,30.2,-59.2,-167.9,39,529327)
	spawnMobile("dantooine", "janta_rockshaper",180,20.4,-61.4,-183.7,7,529327)
	spawnMobile("dantooine", "janta_rockshaper",180,23.8,-60.5,-183.2,-18,529327)
	spawnMobile("dantooine", "janta_rockshaper",180,28.2,-60.2,-182.1,-2,529327)
	spawnMobile("dantooine", "janta_rockshaper",180,31.8,-59.7,-183.9,-28,529327)
	spawnMobile("dantooine", "janta_rockshaper",180,34.7,-59.2,-179.0,99, 529327)
	spawnMobile("dantooine", "janta_rockshaper",180,58.6,-56.8,-178.0,-10, 529327)
	spawnMobile("dantooine", "janta_rockshaper",180,65.0,-56.9,-177.4,-28, 529327)
	spawnMobile("dantooine", "janta_rockshaper",180,67.0,-58.6,-167.8,-80, 529327)
end
