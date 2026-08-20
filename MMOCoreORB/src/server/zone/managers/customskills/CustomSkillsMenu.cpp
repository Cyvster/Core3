#include "CustomSkillsMenu.h"
#include "CustomSkillsSuiCallback.h"

#include "server/zone/managers/player/BadgeList.h"
#include "server/zone/managers/stringid/StringIdManager.h"
#include "server/zone/objects/player/PlayerObject.h"
#include "server/zone/objects/player/sui/SuiWindowType.h"

namespace {
#define BADGES(name, ...) const char* const name[] = {__VA_ARGS__}
	BADGES(milestones, "count_5", "count_10", "count_25", "count_50", "count_75", "count_100", "count_125");
	BADGES(explorationMilestones, "bdg_exp_10_badges", "bdg_exp_20_badges", "bdg_exp_30_badges", "bdg_exp_40_badges", "bdg_exp_45_badges");
	BADGES(corellia, "exp_cor_agrilat_swamp", "bdg_exp_cor_bela_vistal_fountain", "bdg_exp_cor_rebel_hideout", "bdg_exp_cor_rogue_corsec_base", "bdg_exp_cor_tyrena_theater");
	BADGES(dantooine, "bdg_exp_dan_dantari_village1", "bdg_exp_dan_dantari_village2", "exp_dan_jedi_temple", "exp_dan_rebel_base");
	BADGES(dathomir, "bdg_exp_dat_crashed_ship", "exp_dat_escape_pod", "bdg_exp_dat_imp_prison", "exp_dat_misty_falls_1", "exp_dat_misty_falls_2", "exp_dat_sarlacc", "exp_dat_tarpit");
	BADGES(endor, "bdg_exp_end_dulok_village", "bdg_exp_end_ewok_lake_village", "bdg_exp_end_ewok_tree_village", "bdg_exp_end_imp_outpost");
	BADGES(lok, "bdg_exp_lok_imp_outpost", "bdg_exp_lok_kimogila_skeleton", "exp_lok_volcano");
	BADGES(naboo, "bdg_exp_nab_amidalas_sandy_beach", "bdg_exp_nab_deeja_falls_top", "exp_nab_gungan_sacred_place", "bdg_exp_nab_theed_falls_bottom");
	BADGES(rori, "bdg_exp_ror_imp_camp", "bdg_exp_ror_imp_hyperdrive_fac", "bdg_exp_ror_kobala_spice_mine", "bdg_exp_ror_rebel_outpost");
	BADGES(talus, "bdg_exp_tal_aqualish_cave", "bdg_exp_tal_creature_village", "bdg_exp_tal_imp_base", "bdg_exp_tal_imp_vs_reb_battle");
	BADGES(tatooine, "exp_tat_bens_hut", "exp_tat_escape_pod", "exp_tat_krayt_graveyard", "exp_tat_krayt_skeleton", "exp_tat_lars_homestead", "exp_tat_sarlacc_pit", "exp_tat_tusken_pool");
	BADGES(yavin4, "exp_yav_temple_exar_kun", "exp_yav_temple_blueleaf", "exp_yav_temple_woolamander");
	BADGES(combat, "combat_1hsword_master", "combat_2hsword_master", "combat_bountyhunter_master", "combat_brawler_master", "combat_carbine_master", "combat_commando_master", "combat_marksman_master", "combat_pistol_master", "combat_polearm_master", "combat_rifleman_master", "combat_smuggler_master", "combat_unarmed_master");
	BADGES(crafting, "crafting_architect_master", "crafting_armorsmith_master", "crafting_artisan_master", "crafting_chef_master", "crafting_droidengineer_master", "crafting_merchant_master", "crafting_shipwright", "crafting_tailor_master", "crafting_weaponsmith_master");
	BADGES(outdoors, "outdoors_bio_engineer_master", "outdoors_creaturehandler_master", "outdoors_ranger_master", "outdoors_scout_master", "outdoors_squadleader_master");
	BADGES(science, "science_combatmedic_master", "science_doctor_master", "science_medic_master");
	BADGES(social, "social_dancer_master", "social_entertainer_master", "social_imagedesigner_master", "social_musician_master", "social_politician_master");
	BADGES(pilot, "pilot_imperial_navy_corellia", "pilot_imperial_navy_naboo", "pilot_imperial_navy_tatooine", "pilot_neutral_corellia", "pilot_neutral_naboo", "pilot_neutral_tatooine", "pilot_rebel_navy_corellia", "pilot_rebel_navy_naboo", "pilot_rebel_navy_tatooine");
	BADGES(hero, "poi_rabidbeast", "poi_prisonbreak", "poi_twoliars", "poi_factoryliberation", "poi_heromark");
	BADGES(warren, "warren_compassion", "warren_hero");
	BADGES(themeParks, "bdg_thm_park_jabba_badge", "bdg_thm_park_imperial_badge", "bdg_thm_park_rebel_badge", "bdg_thm_park_nym_badge");
	BADGES(corvette, "bdg_corvette_imp_destroy", "bdg_corvette_imp_rescue", "bdg_corvette_imp_assassin", "bdg_corvette_neutral_destroy", "bdg_corvette_neutral_rescue", "bdg_corvette_neutral_assassin", "bdg_corvette_reb_destroy", "bdg_corvette_reb_rescue", "bdg_corvette_reb_assassin");
	BADGES(coa, "event_coa2_rebel", "event_coa2_imperial", "event_coa3_rebel", "event_coa3_imperial", "event_project_dead_eye_1");
	BADGES(accolades, "acc_brave_soldier", "acc_fascinating_background", "acc_good_samaritan", "acc_interesting_personage", "acc_professional_demeanor", "bdg_accolade_home_show", "bdg_accolade_live_event");
	BADGES(librarian, "bdg_library_trivia");
	BADGES(racing, "bdg_racing_agrilat_swamp", "bdg_racing_keren_city", "bdg_racing_mos_espa", "bdg_racing_lok_marathon", "bdg_racing_narmle_memorial", "bdg_racing_nashal_river");
	BADGES(deathStar, "destroy_deathstar");
#undef BADGES
	template <int N> int countOf(const char* const (&)[N]) { return N; }
}

void CustomSkillsMenu::open(CreatureObject* player, Page page) {
	if (player == nullptr || player->getPlayerObject() == nullptr)
		return;
	PlayerObject* ghost = player->getPlayerObject();
	int boxType = page == MAIN ? SuiListBox::HANDLETWOBUTTON : SuiListBox::HANDLETHREEBUTTON;
	ManagedReference<SuiListBox*> box = new SuiListBox(player, SuiWindowType::NONE, boxType);
	box->setCallback(new CustomSkillsSuiCallback(player->getZoneServer(), page));
	box->setCancelButton(true, "@cancel");
	box->setOkButton(true, "@ok");
	box->setPromptTitle(getTitle(page));
	box->setPromptText(page == MAIN ? "Select a category." : "Select an entry to continue.");
	if (page != MAIN)
		box->setOtherButton(true, "@back");
	addPageItems(box, player, page);
	ghost->addSuiBox(box);
	player->sendMessage(box->generateMessage());
}

void CustomSkillsMenu::addBadgeItems(SuiListBox* box, CreatureObject* player, const char* const* keys, int count) {
	PlayerObject* ghost = player->getPlayerObject();
	const BadgeList* list = BadgeList::instance();
	for (int i = 0; i < count; ++i) {
		const Badge* badge = list->get(keys[i]);
		if (badge == nullptr)
			continue;
		String marker = ghost->hasBadge(badge->getIndex()) ? "\\#00FF00O" : "\\#FF0000X";
		String stringId = "@badge_n:" + badge->getKey();
		String badgeName = StringIdManager::instance()->getStringId(stringId.hashCode()).toString();

		// Keep the stable badge key visible if a custom server is missing the
		// corresponding STF entry instead of producing a blank menu row.
		if (badgeName.isEmpty())
			badgeName = badge->getKey();

		box->addMenuItem(marker + " \\#.  " + badgeName);
	}
}

void CustomSkillsMenu::addPageItems(SuiListBox* box, CreatureObject* player, Page page) {
#define LEAF(pageName, data) case pageName: addBadgeItems(box, player, data, countOf(data)); break
	switch (page) {
	case MAIN: box->addMenuItem("Badges"); break;
	case BADGES: box->addMenuItem("Milestone Badges"); box->addMenuItem("Exploration"); box->addMenuItem("Profession"); box->addMenuItem("Quest"); box->addMenuItem("Event"); break;
	case EXPLORATION: box->addMenuItem("Milestone Exploration"); box->addMenuItem("Corellia"); box->addMenuItem("Dantooine"); box->addMenuItem("Dathomir"); box->addMenuItem("Endor"); box->addMenuItem("Lok"); box->addMenuItem("Naboo"); box->addMenuItem("Rori"); box->addMenuItem("Talus"); box->addMenuItem("Tatooine"); box->addMenuItem("Yavin IV"); break;
	case PROFESSION: box->addMenuItem("Combat"); box->addMenuItem("Crafting"); box->addMenuItem("Outdoors"); box->addMenuItem("Science"); box->addMenuItem("Social"); box->addMenuItem("Pilot"); break;
	case QUEST: box->addMenuItem("Hero of Tatooine"); box->addMenuItem("Warren"); box->addMenuItem("Theme Parks"); box->addMenuItem("Corellian Corvette"); break;
	case EVENT: box->addMenuItem("Cries of Alderaan"); box->addMenuItem("Accolades"); box->addMenuItem("Librarian"); box->addMenuItem("Racing"); box->addMenuItem("Death Star"); break;
	LEAF(MILESTONES, milestones); LEAF(EXPLORATION_MILESTONES, explorationMilestones); LEAF(CORELLIA, corellia); LEAF(DANTOOINE, dantooine); LEAF(DATHOMIR, dathomir); LEAF(ENDOR, endor); LEAF(LOK, lok); LEAF(NABOO, naboo); LEAF(RORI, rori); LEAF(TALUS, talus); LEAF(TATOOINE, tatooine); LEAF(YAVIN4, yavin4);
	LEAF(PROFESSION_COMBAT, combat); LEAF(PROFESSION_CRAFTING, crafting); LEAF(PROFESSION_OUTDOORS, outdoors); LEAF(PROFESSION_SCIENCE, science); LEAF(PROFESSION_SOCIAL, social); LEAF(PROFESSION_PILOT, pilot);
	LEAF(QUEST_HERO, hero); LEAF(QUEST_WARREN, warren); LEAF(QUEST_THEME_PARKS, themeParks); LEAF(QUEST_CORVETTE, corvette);
	LEAF(EVENT_COA, coa); LEAF(EVENT_ACCOLADES, accolades); LEAF(EVENT_LIBRARIAN, librarian); LEAF(EVENT_RACING, racing); LEAF(EVENT_DEATH_STAR, deathStar);
	}
#undef LEAF
}

CustomSkillsMenu::Page CustomSkillsMenu::getChild(Page page, int selection) {
	static const Page main[] = {BADGES};
	static const Page badges[] = {MILESTONES, EXPLORATION, PROFESSION, QUEST, EVENT};
	static const Page exploration[] = {EXPLORATION_MILESTONES, CORELLIA, DANTOOINE, DATHOMIR, ENDOR, LOK, NABOO, RORI, TALUS, TATOOINE, YAVIN4};
	static const Page profession[] = {PROFESSION_COMBAT, PROFESSION_CRAFTING, PROFESSION_OUTDOORS, PROFESSION_SCIENCE, PROFESSION_SOCIAL, PROFESSION_PILOT};
	static const Page quest[] = {QUEST_HERO, QUEST_WARREN, QUEST_THEME_PARKS, QUEST_CORVETTE};
	static const Page event[] = {EVENT_COA, EVENT_ACCOLADES, EVENT_LIBRARIAN, EVENT_RACING, EVENT_DEATH_STAR};
	if (selection < 0) return page;
#define CHILD(parent, data) case parent: return selection < static_cast<int>(sizeof(data) / sizeof(data[0])) ? data[selection] : page
	switch (page) { CHILD(MAIN, main); CHILD(BADGES, badges); CHILD(EXPLORATION, exploration); CHILD(PROFESSION, profession); CHILD(QUEST, quest); CHILD(EVENT, event); default: return page; }
#undef CHILD
}

CustomSkillsMenu::Page CustomSkillsMenu::getParent(Page page) {
	if (page == BADGES) return MAIN;
	if (page >= MILESTONES && page <= EVENT) return BADGES;
	if (page >= EXPLORATION_MILESTONES && page <= YAVIN4) return EXPLORATION;
	if (page >= PROFESSION_COMBAT && page <= PROFESSION_PILOT) return PROFESSION;
	if (page >= QUEST_HERO && page <= QUEST_CORVETTE) return QUEST;
	if (page >= EVENT_COA && page <= EVENT_DEATH_STAR) return EVENT;
	return MAIN;
}

String CustomSkillsMenu::getTitle(Page page) {
	static const char* const titles[] = {"Custom Skills", "Custom Skills > Badges", "Badges > Milestone Badges", "Badges > Exploration", "Badges > Profession", "Badges > Quest", "Badges > Event", "Exploration > Milestone Exploration", "Exploration > Corellia", "Exploration > Dantooine", "Exploration > Dathomir", "Exploration > Endor", "Exploration > Lok", "Exploration > Naboo", "Exploration > Rori", "Exploration > Talus", "Exploration > Tatooine", "Exploration > Yavin IV", "Profession > Combat", "Profession > Crafting", "Profession > Outdoors", "Profession > Science", "Profession > Social", "Profession > Pilot", "Quest > Hero of Tatooine", "Quest > Warren", "Quest > Theme Parks", "Quest > Corellian Corvette", "Event > Cries of Alderaan", "Event > Accolades", "Event > Librarian", "Event > Racing", "Event > Death Star"};
	return titles[static_cast<int>(page)];
}
