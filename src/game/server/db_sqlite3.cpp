#include "db_sqlite3.h"
#include <base/system.h> //ChillerDragon needs for str_len func
#include <fstream> //ChillerDragon much wow shitty filestream usage -.-
#include <string> //ChillerDragon ikr no kewl C style stuff

bool CQuery::Next()
{
#if defined(CONF_DEBUG)
	CALL_STACK_ADD();
#endif
	int Ret = sqlite3_step(m_pStatement);
	return Ret == SQLITE_ROW;
}

void CQuery::Query(CSql *pDatabase, char *pQuery)
{
#if defined(CONF_DEBUG)
	CALL_STACK_ADD();
	if (!pQuery) {
		dbg_msg("cBug","[WARNING] no pQuery found in Query() function line 11.");
	}
#endif
	m_pDatabase = pDatabase;
	m_pDatabase->Query(this, pQuery);
}

void CQuery::OnData()
{
#if defined(CONF_DEBUG)
	CALL_STACK_ADD();
#endif
	Next();
}

int CQuery::GetID(const char *pName)
{
#if defined(CONF_DEBUG)
	CALL_STACK_ADD();
#endif
	for (int i = 0; i < GetColumnCount(); i++)
	{
		if (str_comp(GetName(i), pName) == 0)
			return i;
	}
	return -1;
}

void CSql::WorkerThread()
{
#if defined(CONF_DEBUG)
	CALL_STACK_ADD();
#endif
	while (m_Running)
	{
		lock_wait(m_Lock); //lock queue
		if (m_lpQueries.size() > 0)
		{
			CQuery *pQuery = m_lpQueries.front();
			m_lpQueries.pop();
			lock_unlock(m_Lock); //unlock queue

			int Ret;
			Ret = sqlite3_prepare_v2(m_pDB, pQuery->m_Query.c_str(), -1, &pQuery->m_pStatement, 0);
			if (Ret == SQLITE_OK)
			{
				if (!m_Running) //last check
					break;

                // pQuery->OnData()

                lock_wait(m_CallbackLock);
                m_lpExecutedQueries.push(pQuery);
                lock_unlock(m_CallbackLock);

				// sqlite3_finalize(pQuery->m_pStatement);
			}
			else
				dbg_msg("SQLite", "%s", sqlite3_errmsg(m_pDB));

			// delete pQuery;
		}
		else
		{
			thread_sleep(100);
			lock_unlock(m_Lock); //unlock queue
		}

		thread_sleep(10);
	}
}

void CSql::Tick()
{
    while(true) {
        lock_wait(m_CallbackLock);
        if(!m_lpExecutedQueries.size()) {
            lock_unlock(m_CallbackLock);
            break;
        }

        CQuery *pQuery = m_lpExecutedQueries.front();
        m_lpExecutedQueries.pop();

        pQuery->OnData();

        sqlite3_finalize(pQuery->m_pStatement);

        lock_unlock(m_CallbackLock);
        delete pQuery;
    }
}

void CSql::InitWorker(void *pUser)
{
#if defined(CONF_DEBUG)
	CALL_STACK_ADD();
#endif
	CSql *pSelf = (CSql *)pUser;
	pSelf->WorkerThread();
}

CQuery *CSql::Query(CQuery *pQuery, std::string QueryString)
{
#if defined(CONF_DEBUG)
	CALL_STACK_ADD();
	if (!pQuery) {
		dbg_msg("cBug", "[WARNING] no pQuery found in CQuery *CSql::Query(CQuery *pQuery, std::string QueryString) function line 90.");
		return NULL;
	}
#endif

	pQuery->m_Query = QueryString;

	lock_wait(m_Lock);
	m_lpQueries.push(pQuery);
	lock_unlock(m_Lock);

	return pQuery;
}

CSql::CSql()
{
	sqlite3 *test;

	std::ifstream f;
	std::string aUserInputLoL;

	f.open("database_path.txt", std::ios::in);
	if (getline(f, aUserInputLoL))
		dbg_msg("SQLite", "successfully loaded path '%s'", aUserInputLoL.c_str());
	else
		dbg_msg("SQLite","error reading database pathfile 'database_path.txt'");
	f.close();
	dbg_msg("SQLite", "connecting to '%s' ", aUserInputLoL.c_str());
	int rc = sqlite3_open(aUserInputLoL.c_str(), &m_pDB);
	if (rc)
	{
		dbg_msg("SQLite", "Can't open database error: %d", rc);
		sqlite3_close(m_pDB);
	}

	char *Query = "CREATE TABLE IF NOT EXISTS Accounts (" \
		"ID							INTEGER			PRIMARY KEY		AUTOINCREMENT," \
		"Username					VARCHAR(32)		NOT NULL," \
		"Password					VARCHAR(128)	NOT NULL," \
		"IsLoggedIn					INTEGER			DEFAULT 0," \
		"LastLoginPort				INTEGER			DEFAULT 0," \
		"LastLogoutIGN1				VARCHAR(32)		DEFAULT ''," \
		"LastLogoutIGN2				VARCHAR(32)		DEFAULT ''," \
		"LastLogoutIGN3				VARCHAR(32)		DEFAULT ''," \
		"LastLogoutIGN4				VARCHAR(32)		DEFAULT ''," \
		"LastLogoutIGN5				VARCHAR(32)		DEFAULT ''," \
		"IP_1						VARCHAR(32)		DEFAULT ''," \
		"IP_2						VARCHAR(32)		DEFAULT ''," \
		"IP_3						VARCHAR(32)		DEFAULT ''," \
		"Clan1						VARCHAR(32)		DEFAULT ''," \
		"Clan2						VARCHAR(32)		DEFAULT ''," \
		"Clan3						VARCHAR(32)		DEFAULT ''," \
		"Skin						VARCHAR(32)		DEFAULT ''," \
		"Level						INTEGER			DEFAULT 0," \
		"Money						INTEGER			DEFAULT 0," \
		"Exp						INTEGER			DEFAULT 0," \
		"Shit						INTEGER			DEFAULT 0," \
		"LastGift					INTEGER			DEFAULT 0," \
		"PoliceRank					INTEGER			DEFAULT 0," \
		"JailTime					INTEGER			DEFAULT 0," \
		"EscapeTime					INTEGER			DEFAULT 0," \
		"TaserLevel					INTEGER			DEFAULT 0," \
		"PvPArenaTickets			INTEGER			DEFAULT 0," \
		"PvPArenaGames				INTEGER			DEFAULT 0," \
		"PvPArenaKills				INTEGER			DEFAULT 0," \
		"PvPArenaDeaths				INTEGER			DEFAULT 0," \
		"ProfileStyle				INTEGER			DEFAULT 0," \
		"ProfileViews				INTEGER			DEFAULT 0," \
		"ProfileStatus				VARCHAR(128)	DEFAULT ''," \
		"ProfileSkype				VARCHAR(128)	DEFAULT ''," \
		"ProfileYoutube				VARCHAR(128)	DEFAULT ''," \
		"ProfileEmail				VARCHAR(128)	DEFAULT ''," \
		"ProfileHomepage			VARCHAR(128)	DEFAULT ''," \
		"ProfileTwitter				VARCHAR(128)	DEFAULT ''," \
		"HomingMissiles				INTEGER			DEFAULT 0," \
		"BlockPoints				INTEGER			DEFAULT 0," \
		"BlockKills					INTEGER			DEFAULT 0," \
		"BlockDeaths				INTEGER			DEFAULT 0," \
		"BlockSkill					INTEGER			DEFAULT 0," \
		"IsModerator				INTEGER			DEFAULT 0," \
		"IsSuperModerator			INTEGER			DEFAULT 0," \
		"IsSupporter				INTEGER			DEFAULT 0," \
		"IsAccFrozen				INTEGER			DEFAULT 0," \
		"BombGamesPlayed			INTEGER			DEFAULT 0," \
		"BombGamesWon				INTEGER			DEFAULT 0," \
		"BombBanTime				INTEGER			DEFAULT 0," \
		"GrenadeKills				INTEGER			DEFAULT 0," \
		"GrenadeDeaths				INTEGER			DEFAULT 0," \
		"GrenadeSpree				INTEGER			DEFAULT 0," \
		"GrenadeShots				INTEGER			DEFAULT 0," \
		"GrenadeShotsNoRJ			INTEGER			DEFAULT 0," \
		"GrenadeWins				INTEGER			DEFAULT 0," \
		"RifleKills					INTEGER			DEFAULT 0," \
		"RifleDeaths				INTEGER			DEFAULT 0," \
		"RifleSpree					INTEGER			DEFAULT 0," \
		"RifleShots					INTEGER			DEFAULT 0," \
		"RifleWins					INTEGER			DEFAULT 0," \
		"FngConfig					VARCHAR(4)		DEFAULT ''," \
		"ShowHideConfig				VARCHAR(16)		DEFAULT '0010000000'," \
		"SurvivalKills				INTEGER			DEFAULT 0," \
		"SurvivalDeaths				INTEGER			DEFAULT 0," \
		"SurvivalWins				INTEGER			DEFAULT 0," \
		"NinjaJetpackBought			INTEGER			DEFAULT 0," \
		"UseSpawnWeapons			INTEGER			DEFAULT 0," \
		"SpawnWeaponShotgun			INTEGER			DEFAULT 0," \
		"SpawnWeaponGrenade 		INTEGER			DEFAULT 0," \
		"SpawnWeaponRifle			INTEGER			DEFAULT 0," \
		"AsciiState					VARCHAR(4)		DEFAULT ''," \
		"AsciiViewsDefault			INTEGER			DEFAULT 0," \
		"AsciiViewsProfile			INTEGER			DEFAULT 0," \
		"AsciiFrame0				VARCHAR(64)		DEFAULT ''," \
		"AsciiFrame1				VARCHAR(64)		DEFAULT ''," \
		"AsciiFrame2				VARCHAR(64)		DEFAULT ''," \
		"AsciiFrame3				VARCHAR(64)		DEFAULT ''," \
		"AsciiFrame4				VARCHAR(64)		DEFAULT ''," \
		"AsciiFrame5				VARCHAR(64)		DEFAULT ''," \
		"AsciiFrame6				VARCHAR(64)		DEFAULT ''," \
		"AsciiFrame7				VARCHAR(64)		DEFAULT ''," \
		"AsciiFrame8				VARCHAR(64)		DEFAULT ''," \
		"AsciiFrame9				VARCHAR(64)		DEFAULT ''," \
		"AsciiFrame10				VARCHAR(64)		DEFAULT ''," \
		"AsciiFrame11				VARCHAR(64)		DEFAULT ''," \
		"AsciiFrame12				VARCHAR(64)		DEFAULT ''," \
		"AsciiFrame13				VARCHAR(64)		DEFAULT ''," \
		"AsciiFrame14				VARCHAR(64)		DEFAULT ''," \
		"AsciiFrame15				VARCHAR(64)		DEFAULT '');";


	sqlite3_exec(m_pDB, Query, 0, 0, 0);

	m_Lock = lock_create();
	m_CallbackLock = lock_create();
	m_Running = true;
	thread_init(InitWorker, this);
}

CSql::~CSql()
{
#if defined(CONF_DEBUG)
	CALL_STACK_ADD();
#endif
	m_Running = false;
	lock_wait(m_Lock);

	while (m_lpQueries.size())
	{
		CQuery *pQuery = m_lpQueries.front();
		m_lpQueries.pop();
		delete pQuery;
	}

	lock_unlock(m_Lock);
	lock_destroy(m_Lock);

	lock_wait(m_CallbackLock);

		while (m_lpExecutedQueries.size()) {
    	    CQuery *pQuery = m_lpExecutedQueries.front();
            m_lpExecutedQueries.pop();
            delete pQuery;
    	}

    lock_unlock(m_CallbackLock);
    lock_destroy(m_CallbackLock);
}
