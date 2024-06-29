#include <Geode/Geode.hpp>
#include <Geode/ui/BasedButtonSprite.hpp>

using namespace geode::prelude;

CCMenuItemSpriteExtra* createNewCLbutton(std::string title, std::string description, std::string sprite, cocos2d::SEL_MenuHandler callback) {
	CCNode* spriteNode = CCNode::create();

	CCScale9Sprite* buttonSpr1 = CCScale9Sprite::create("longButtonHalf-1.png"_spr);
	buttonSpr1->setContentSize({150.f, 30.f});
	buttonSpr1->setPosition(ccp(75.f, 44.f));
	spriteNode->addChild(buttonSpr1);

	CCScale9Sprite* buttonSpr2 = CCScale9Sprite::create("longButtonHalf-2.png"_spr);
	buttonSpr2->setContentSize({150.f, 30.f});
	buttonSpr2->setPosition(ccp(75.f, 14.f));
	spriteNode->addChild(buttonSpr2);

	if (sprite != "N/A") {
		CCSprite* icon = CCSprite::createWithSpriteFrameName(sprite.c_str());
		icon->setPosition(ccp(122, 29));
		icon->setScale(1.25f);
		spriteNode->addChild(icon);
	}

	CCLabelBMFont* titleLabel = CCLabelBMFont::create(title.c_str(), "bigFont.fnt");
	titleLabel->setScale(0.5f);
	titleLabel->setAnchorPoint(ccp(0, 0));
	titleLabel->setPosition(ccp(5, 40));
	spriteNode->addChild(titleLabel);

	CCLabelBMFont* descLabel = CCLabelBMFont::create(description.c_str(), "NewPusab.fnt"_spr);
	descLabel->setPosition(ccp(6, 20));
	descLabel->setAnchorPoint(ccp(0,0));
	descLabel->setScale(0.3f);
	descLabel->setOpacity(150.f);
	descLabel->setColor(ccc3(0, 0, 0));
	spriteNode->addChild(descLabel);

	//spriteNode->setScale(0.8f);

	auto button = CCMenuItemSpriteExtra::create(spriteNode, spriteNode, callback);
	button->setContentSize({150.f, 60.f});
	button->m_scaleMultiplier = 1.1f;

	return button;
}

// For some reason, click the back button on the GauntletSelectLayer (a.k.a the Gauntlet menu) 
// ALWAYS returns to CreatorLayer (a.k.a the menu after clicking the creators button on MenuLayer)
// So this hook just fixes that and returns us back to our modified MenuLayer

#include <Geode/modify/GauntletSelectLayer.hpp>
class $modify(GauntletSelectLayer) {
	void onBack(cocos2d::CCObject* sender) {
		CCScene* sc = CCScene::create();
		sc->addChild(MenuLayer::scene(false));
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
	}
};

// Now the REAL magic happens

#include <Geode/modify/MenuLayer.hpp>
class $modify(CrazyLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) {
			return false;
		}

		// MAIN MENU CHANGES (MIGHT BE BREAKING SOME STUFF)

		this->getChildByID("main-title")->setScale(0.8f);
		this->getChildByID("main-title")->setPositionY(this->getChildByID("main-title")->getPositionY() - (Mod::get()->getSettingValue<bool>("remove-account-stats") ? 10.f : 20.f));
		this->getChildByID("main-title")->setPositionX(256.5f);

		this->getChildByID("more-games-menu")->setVisible(false);
		this->getChildByID("social-media-menu")->setVisible(false); // might make RobTop logo visible later
		this->getChildByID("main-menu")->setVisible(false); // hehehehe >:D

		this->getChildByID("bottom-menu")->setPosition(ccp(540.f, 190.f));
		this->getChildByID("bottom-menu")->setScale(0.75f);
		this->getChildByID("bottom-menu")->setLayout(ColumnLayout::create()->setAxisReverse(true)->setAutoScale(true)->setGap(0.f)->setCrossAxisOverflow(true));
		this->getChildByID("bottom-menu")->setContentHeight(305.f);
		this->getChildByID("bottom-menu")->updateLayout();
		this->getChildByID("bottom-menu")->setZOrder(1);

		this->getChildByID("right-side-menu")->setPosition(ccp(138.f, 25.f));
		this->getChildByID("right-side-menu")->setScale(0.75f);
		this->getChildByID("right-side-menu")->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start)->setAxisReverse(true));
		this->getChildByID("right-side-menu")->setContentWidth(305.f);
		this->getChildByID("right-side-menu")->updateLayout();

		this->getChildByID("right-side-menu")->getChildByID("daily-chest-button")->setZOrder(4);

		auto questBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("RD_quests.png"_spr), this, menu_selector(CrazyLayer::onQuest));
		this->getChildByID("right-side-menu")->addChild(questBtn, 3);

		auto pathsBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("RD_paths.png"_spr), this, menu_selector(CrazyLayer::onPaths));
		this->getChildByID("right-side-menu")->addChild(pathsBtn, 2);

		auto mapPacksBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("RD_mappacks.png"_spr), this, menu_selector(CrazyLayer::onMPs));
		this->getChildByID("right-side-menu")->addChild(mapPacksBtn, 1);

		this->getChildByID("player-username")->setPositionY(this->getChildByID("player-username")->getPositionY() - 75.f);
		this->getChildByID("player-username")->setPositionX(540.f);
		this->getChildByID("player-username")->setScale(this->getChildByID("player-username")->getScale() - 0.1f);
		this->getChildByID("profile-menu")->setPosition(ccp(575.5f, 40.f));
		this->getChildByID("profile-menu")->setScale(0.75f);
		this->getChildByID("profile-menu")->setZOrder(1);

		if (!Mod::get()->getSettingValue<bool>("remove-account-stats")) {
			this->getChildByID("player-username")->setVisible(false);
			this->getChildByID("profile-menu")->setVisible(false);
			this->getChildByID("bottom-menu")->setPositionY(CCDirector::get()->getWinSize().height / 2.f);
			this->getChildByID("bottom-menu")->setContentHeight(390.f);
			this->getChildByID("bottom-menu")->updateLayout();
		}

		// NEW STUFF YAYY :D

		// background for the menu on the right (the one with the Geode and settings buttons)

		CCScale9Sprite* bottomMenuBG = CCScale9Sprite::create("square02b_001.png");
		bottomMenuBG->setColor({0,0,0});
		bottomMenuBG->setOpacity(100);
		bottomMenuBG->setPosition(this->getChildByID("bottom-menu")->getPosition());
		bottomMenuBG->setPositionY(CCDirector::get()->getWinSize().height / 2.f);
		bottomMenuBG->setContentSize({54.f, 302.f});
		addChild(bottomMenuBG);

		// background for the account username thing

		if (!Mod::get()->getSettingValue<bool>("remove-account-stats")) { // then make the account stats (obviously)
			CCMenu* accountUsernameMenu = CCMenu::create();
			accountUsernameMenu->setScale(0.5f);
			accountUsernameMenu->setPosition(200.f, 302.f);
			accountUsernameMenu->setLayout(RowLayout::create()->setAxisAlignment(AxisAlignment::Start)->setAutoScale(false)->setCrossAxisOverflow(false)->setGap(27.f));
			//accountUsernameMenu->setContentWidth(570.f);
			accountUsernameMenu->setContentHeight(53.f);

			SimplePlayer* playerNode = SimplePlayer::create(GameManager::get()->getPlayerFrame());
			playerNode->setColor(GameManager::get()->colorForIdx(GameManager::get()->getPlayerColor()));
			playerNode->setSecondColor(GameManager::get()->colorForIdx(GameManager::get()->getPlayerColor2()));
			playerNode->setScale(1.25f);
			accountUsernameMenu->addChild(playerNode);

			CCLabelBMFont* usernameLabel = CCLabelBMFont::create(GJAccountManager::get()->m_username.c_str(), "goldFont.fnt");
			accountUsernameMenu->addChild(usernameLabel);

			addChild(accountUsernameMenu, 1);
			accountUsernameMenu->updateLayout();

			CCScale9Sprite* accUsrBG = CCScale9Sprite::create("square02b_001.png");
			accUsrBG->setScale(0.5f);
			accUsrBG->setColor({0,0,0});
			accUsrBG->setOpacity(100);
			accUsrBG->setPosition(accountUsernameMenu->getPosition());
			accUsrBG->setContentSize({accountUsernameMenu->getContentWidth() + 55.f, accountUsernameMenu->getContentHeight() + 5.f});
			addChild(accUsrBG);
		}

		CCMenu* timedLevels = CCMenu::create();
		timedLevels->setID("timed-levels-menu"_spr);
		timedLevels->setLayout(ColumnLayout::create()->setGap(10.f));
		timedLevels->setScale(0.7f);
		timedLevels->setPosition({490.f, 200.f});

		timedLevels->addChild(CCMenuItemSpriteExtra::create(CircleButtonSprite::createWithSpriteFrameName("gj_dailyCrown_001.png"), this, menu_selector(CrazyLayer::onDaily)));
		timedLevels->addChild(CCMenuItemSpriteExtra::create(CircleButtonSprite::createWithSpriteFrameName("gj_weeklyCrown_001.png"), this, menu_selector(CrazyLayer::onWeekly)));

		addChild(timedLevels);

		timedLevels->updateLayout();

		CCMenu* mainMenu = CCMenu::create();
		mainMenu->setID("new-main-menu"_spr);
		mainMenu->setContentSize({675.f, 100.f});
		mainMenu->ignoreAnchorPointForPosition(false);
		mainMenu->setPositionX(256.5f);
		mainMenu->setScale(0.75f);
		mainMenu->setPositionY(100.f);
		mainMenu->setLayout(RowLayout::create()->setGap(7.f)->setGrowCrossAxis(true)->setAutoScale(false));

		mainMenu->addChild(createNewCLbutton("Create", "Create your\nown levels!", "RD_create.png"_spr, menu_selector(CrazyLayer::onLocalLevels)));
		mainMenu->addChild(createNewCLbutton("Saved", "Play your\nsaved levels!", "RD_saved.png"_spr, menu_selector(CrazyLayer::onSaved)));
		mainMenu->addChild(createNewCLbutton("Scores", "View the\nrankings!", "RD_leaderboards.png"_spr, menu_selector(CrazyLayer::onLeaderboard)));
		mainMenu->addChild(createNewCLbutton("Featured", "Play new\nfeatured levels!", "RD_featured.png"_spr, menu_selector(CrazyLayer::onFeatured)));
		mainMenu->addChild(createNewCLbutton("Gauntlets", "Play the\nnew gauntlets!", "RD_gauntlets.png"_spr, menu_selector(CrazyLayer::onGauntlet)));
		mainMenu->addChild(createNewCLbutton("Lists", "Play rated\nlists!", "RD_lists.png"_spr, menu_selector(CrazyLayer::onList)));
		mainMenu->addChild(createNewCLbutton("Search", "Search for\nlevels online!", "RD_search.png"_spr, menu_selector(CrazyLayer::onSearch)));

		addChild(mainMenu, 99);

		mainMenu->updateLayout();

		return true;
	}

	void onLocalLevels(CCObject*) {
		GJSearchObject* so = GJSearchObject::create(SearchType::MyLevels);
		CCScene* sc = CCScene::create();
		sc->addChild(LevelBrowserLayer::create(so));
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
	}

	void onList(CCObject*) {
		GJSearchObject* so = GJSearchObject::create(SearchType::Featured);
		so->m_searchMode = 1;
		CCScene* sc = CCScene::create();
		sc->addChild(LevelBrowserLayer::create(so));
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
	}

	void onSaved(CCObject*) {
		GJSearchObject* so = GJSearchObject::create(SearchType::SavedLevels);
		CCScene* sc = CCScene::create();
		sc->addChild(LevelBrowserLayer::create(so));
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
	}

	void onLeaderboard(CCObject*) {
		CCScene* sc = CCScene::create();
		sc->addChild(LeaderboardsLayer::create(LeaderboardState::Default));
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
	}

	void onGauntlet(CCObject*) {
		CCScene* sc = CCScene::create();
		sc->addChild(GauntletSelectLayer::create(1));
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
	}

	void onFeatured(CCObject*) {
		GJSearchObject* so = GJSearchObject::create(SearchType::Featured);
		CCScene* sc = CCScene::create();
		sc->addChild(LevelBrowserLayer::create(so));
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
	}

	void onSearch(CCObject*) {
		CCScene* sc = CCScene::create();
		sc->addChild(LevelSearchLayer::create(0));
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
	}

	void onQuest(CCObject*) {
		ChallengesPage::create()->show();
	}

	void onMPs(CCObject*) {
		GJSearchObject* so = GJSearchObject::create(SearchType::MapPack);
		CCScene* sc = CCScene::create();
		sc->addChild(LevelBrowserLayer::create(so));
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, sc));
	}

	void onPaths(CCObject*) {
		GJPathsLayer::create()->show();
	}

	void onDaily(CCObject*) {
		DailyLevelPage::create(GJTimedLevelType::Daily)->show();
	}

	void onWeekly(CCObject*) {
		DailyLevelPage::create(GJTimedLevelType::Weekly)->show();
	}
};
