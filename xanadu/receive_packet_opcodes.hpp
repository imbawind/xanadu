//

#pragma once

namespace receive_headers_login
{
	constexpr short kLoginRequest = 0x01;
	constexpr short kWORLD_BACK = 0x04;
	constexpr short kCHANNEL_SELECT = 0x05;
	constexpr short kWORLD_SELECT = 0x06;
	constexpr short kHANDLE_LOGIN = 0x09;
	constexpr short kSHOW_WORLD = 0x0b;
	constexpr short kBACK_TO_WORLD = 0x0c;
	constexpr short kVIEW_ALL_CHARS_REQUEST = 0x0d;
	constexpr short kVIEW_ALL_CHARS_CONNECT = 0x0e;
	constexpr short kVIEW_ALL_CHARS = 0x0f;
	constexpr short kCHAR_SELECT = 0x13;
	constexpr short kPLAYER_LOGGEDIN = 0x14;
	constexpr short kNAME_CHECK = 0x15;
	constexpr short kCREATE_CHARACTER = 0x16;
	constexpr short kDELETE_CHARACTER = 0x17;
	constexpr short kLOGIN_BACK = 0x1C;
}

namespace receive_headers
{
	constexpr short kREQUEST_MAP_CHANGE = 0x23;
	constexpr short kCHANGE_CHANNEL = 0x24;
	constexpr short kENTER_CASHSHOP = 0x25;
	constexpr short kMOVE_PLAYER = 0x26;
	constexpr short kCANCEL_CHAIR = 0x27;
	constexpr short kUSE_CHAIR = 0x28;
	constexpr short kREQUEST_CLOSE_RANGE_ATTACK = 0x29;
	constexpr short kRANGED_ATTACK = 0x2a;
	constexpr short kMAGIC_ATTACK = 0x2b;
	constexpr short kENERGY_ATTACK = 0x2c;
	constexpr short kREQUEST_TAKE_DAMAGE = 0x2d;
	constexpr short kGENERAL_CHAT = 0x2e;
	constexpr short kCLOSE_CHALKBOARD = 0x2f;
	constexpr short kFACE_EXPRESSION = 0x30;
	constexpr short kUSE_ITEMEFFECT = 0x31;
	constexpr short kHANDLE_NPC = 0x36;
	constexpr short kNPC_CHAT = 0x38;
	constexpr short kNPC_SHOP = 0x39;
	constexpr short kSTORAGE = 0x3a;
	constexpr short kHIRED_MERCHANT_REQUEST = 0x3b;
	constexpr short kFREDRICK_OPERATION = 0x3c;
	constexpr short kITEM_SORT = 0x40;
	constexpr short kMOVE_ITEM = 0x42;
	constexpr short kUSE_ITEM = 0x43;
	constexpr short kCANCEL_ITEM_BUFF = 0x44;
	constexpr short kUSE_SUMMON_BAG = 0x46;
	constexpr short kUSE_PET_FOOD = 0x47;
	constexpr short kUSE_CASH_ITEM = 0x49;
	constexpr short kUSE_SKILL_BOOK = 0x4b;
	constexpr short kUSE_RETURN_SCROLL = 0x4e;
	constexpr short kUSE_SCROLL = 0x4f;
	constexpr short kDISTRIBUTE_AP = 0x50;
	constexpr short kREQUEST_HEAL_OVER_TIME = 0x51;
	constexpr short kDISTRIBUTE_SP = 0x52;
	constexpr short kUSE_SKILL = 0x53;
	constexpr short kREQUEST_CANCEL_BUFF = 0x54;
	constexpr short kSKILL_EFFECT = 0x55;
	constexpr short kMESO_DROP = 0x56;
	constexpr short kGIVE_FAME = 0x57;
	constexpr short kREQUEST_CHARACTER_INFO = 0x59;
	constexpr short kSPAWN_PET = 0x5a;
	constexpr short kCHANGE_MAP_SPECIAL = 0x5c;
	constexpr short kQUEST_ACTION = 0x62;
	constexpr short kMULTI_CHAT = 0x6B;
	constexpr short kWHISPER = 0x6C;
	constexpr short kMESSENGER = 0x6E;
	constexpr short kPLAYER_INTERACTION = 0x6F;
	constexpr short kPARTY_OPERATION = 0x70;
	constexpr short kPARTY_REQUEST_ANSWER = 0x71;
	constexpr short kGUILD_OPERATION = 0x72;
	constexpr short kGUILD_DENIE = 0x73;
	constexpr short kBUDDY_LIST = 0x76;
	constexpr short kMOVE_PET = 0x8C;
	constexpr short kPET_CHAT = 0x8D;
	constexpr short kPET_COMMAND = 0x8E;
	constexpr short kPET_LOOT = 0x8F;
	constexpr short kSUMMON_MOVE = 0x94;
	constexpr short kDAMAGE_MOB_SUMMON = 0x95;
	constexpr short kSUMMON_DAMAGE = 0x96;
	constexpr short kUSE_DOOR = 0x79;
	constexpr short kCHANGE_KEYMAP = 0x7B;
	constexpr short kGUILD_BBS_OPERATION = 0x86;
	constexpr short kENTER_MTS = 0x87;
	constexpr short kMOVE_LIFE = 0x9D;
	constexpr short kHIT_BY_MOB = 0xA1;
	constexpr short kREQUEST_ITEM_PICKUP = 0xAB;
	constexpr short kHIT_REACTOR = 0xAE;
	constexpr short kCHECK_CASH = 0xC5;
	constexpr short kCASHSHOP_OPERATION = 0xC6;
}
