//

#include "packetcreator.hpp"

#include "player.hpp"
#include "send_packet_opcodes.hpp"

/*
MTS enter packet probably like this:

got it from v0.95 GMS, but verified that it's very likely the same for v0.62 GMS

opcode
writeCharacterData(player);
write<std::string>("test"); // ID name
write<int>(0); // m_nRegisterFeeMeso
write<int>(0); // m_nCommissionRate
write<int>(0); // m_nCommissionBase
write<int>(0); // m_nAuctionDurationMin
write<int>(0); // m_nAuctionDurationMax
write<long long>(0); // time

v2->m_nRegisterFeeMeso = CInPacket::Decode4(v3);
v2->m_nCommissionRate = CInPacket::Decode4(v3);
v2->m_nCommissionBase = CInPacket::Decode4(v3);
v2->m_nAuctionDurationMin = CInPacket::Decode4(v3);
v2->m_nAuctionDurationMax = CInPacket::Decode4(v3);
v757BDB98(&st);
SystemTimeToFileTime(&st, (_FILETIME *)((char *)&ftSecond + 4));
CInPacket::DecodeBuffer(v3, &ftServer, 8u);
v12.dwHighDateTime = ftServer;
v12.dwLowDateTime = (unsigned int)&ftFirst;
v13 = CITC::FileTimeAddition(v2, v12, ftSecond, bSubtract);
v2->m_ftRel.dwLowDateTime = *(_DWORD *)v13.dwLowDateTime;
v2->m_ftRel.dwHighDateTime = *(_DWORD *)(v13.dwLowDateTime + 4);
*/

void PacketCreator::EnterCashShop(Player *player)
{
	write<short>(send_headers::kOPEN_CASHSHOP);
	writeCharacterData(player);
	write<signed char>(1);
	write<std::string>(player->get_user_name());

	/*

	the following data is about removing specific cashshop items from the cashshop totally

	some info for the following write<int>

	  v12 = CInPacket::Decode4(iPacket);
	v13 = v12;
  nNotSaleCount = v12;
  if ( v12 > 0 )
  {
	ZArray<long>::_Alloc((ZArray<long> *)&i, v12, (ZAllocHelper *)&v60);
	CInPacket::DecodeBuffer(iPacket, (void *)i, 4 * v13);
  }
  */
	write<int>(0); // size for nNotSaleCount in CWvsContext::SetSaleInfo

	// loop with the size amount, write SN's 4 bytes int each


	// -------------------------------------------------------------------------------

	/*
	some info for the following write<short>

	 v14 = CInPacket::Decode2(iPacket);
  if ( (signed int)(unsigned __int16)v14 > 0 )
  {
	pExceptionObject = v14;
	do
	{
	  v15 = ZRefCounted_Alloc<CS_COMMODITY>(0);
	  v16 = v15;
	  v17 = &v15->_m_nRef;
	  v15->_m_nRef = 1;
	  v68 = v15;
	  LOBYTE(v75) = 2;
	  v18 = CInPacket::Decode4(iPacket);
	  nSN = v18;
	  v19 = v18;
	  v20 = 0;
	  for ( j = (int)&v5->p; ; j += 8 )
	  {
		if ( !v5 || v20 >= v5[-1].p )
		  goto LABEL_34;
		if ( *(_DWORD *)(*(_DWORD *)j + 12) == v19 )
		  break;
		v20 = (CS_COMMODITY *)((char *)v20 + 1);
	  }
	  CS_COMMODITY::operator=(v16, v65->m_aOriginalCommodity.a[(_DWORD)v20].p);
	  CS_COMMODITY::DecodeModifiedData(v16, iPacket);

	 info about CS_COMMODITY::DecodeModifiedData:

	  void __thiscall CS_COMMODITY::DecodeModifiedData(CS_COMMODITY *this, CInPacket *iPacket)
	  {
	  CS_COMMODITY *v2; // edi@1
	  unsigned int v3; // ebx@1
	  int v4; // eax@2
	  ZArray<long> *v5; // ebx@38
	  int v6; // ebp@38
	  int v7; // edi@39

	  v2 = this;
	  v3 = CInPacket::Decode4(iPacket);
	  if ( v3 & 1 )
	  {
	  v4 = CInPacket::Decode4(iPacket);
	  TSecType<long>::SetData(&v2->nItemId, v4);
	  }
	  if ( v3 & 2 )
	  v2->nCount = CInPacket::Decode2(iPacket);
	  if ( v3 & 0x10 )
	  v2->nPriority = (unsigned __int8)CInPacket::Decode1(iPacket);
	  if ( v3 & 4 )
	  v2->nPrice = CInPacket::Decode4(iPacket);
	  if ( v3 & 8 )
	  v2->bBonus = (unsigned __int8)CInPacket::Decode1(iPacket);
	  if ( v3 & 0x20 )
	  v2->nPeriod = CInPacket::Decode2(iPacket);
	  if ( v3 & 0x20000 )
	  v2->nReqPOP = CInPacket::Decode2(iPacket);
	  if ( v3 & 0x40000 )
	  v2->nReqLEV = CInPacket::Decode2(iPacket);
	  if ( v3 & 0x40 )
	  v2->nMaplePoint = CInPacket::Decode4(iPacket);
	  if ( (v3 & 0x80u) != 0 )
	  v2->nMeso = CInPacket::Decode4(iPacket);
	  if ( v3 & 0x100 )
	  v2->bForPremiumUser = (unsigned __int8)CInPacket::Decode1(iPacket);
	  if ( v3 & 0x200 )
	  v2->nCommodityGender = (unsigned __int8)CInPacket::Decode1(iPacket);
	  if ( v3 & 0x400 )
	  v2->bOnSale = (unsigned __int8)CInPacket::Decode1(iPacket);
	  if ( v3 & 0x800 )
	  v2->nClass = (unsigned __int8)CInPacket::Decode1(iPacket);
	  if ( v3 & 0x1000 )
	  v2->nLimit = (unsigned __int8)CInPacket::Decode1(iPacket);
	  if ( v3 & 0x2000 )
	  v2->nPbCash = CInPacket::Decode2(iPacket);
	  if ( v3 & 0x4000 )
	  v2->nPbPoint = CInPacket::Decode2(iPacket);
	  if ( v3 & 0x8000 )
	  v2->nPbGift = CInPacket::Decode2(iPacket);
	  if ( v3 & 0x10000 )
	  {
	  v5 = &v2->aPackageSN;
	  ZArray<long>::RemoveAll(&v2->aPackageSN);
	  v6 = (unsigned __int8)CInPacket::Decode1(iPacket);
	  if ( v6 > 0 )
	  {
	  do
	  {
	  v7 = CInPacket::Decode4(iPacket);
	  --v6;
	  *ZArray<long>::InsertBefore(v5, -1) = v7;
	  }
	  while ( v6 );
	  }
	  }
	  }

	  some source does it like this:

	  List<SpecialCashItem> lsci = CashItemFactory.getSpecialCashItems();
	  mplew.writeShort(lsci.size());//Guess what
	  for (SpecialCashItem sci : lsci) {
	  mplew.writeInt(sci.getSN());
	  mplew.writeInt(sci.getModifier());
	  mplew.write(sci.getInfo());
	  }

	  probably right except for info (it varies in the way it has byte)
	  and the v6 byte

	  unless these were added later this way

	  */
	write<short>(0); // size for something in CWvsContext::SetSaleInfo

	// -------------------------------------------------------------------------------

	/*
	some info the following write<signed char>
	  v51 = (unsigned __int8)CInPacket::Decode1(iPacket);
  if ( v51 > 0 )
  {
	do
	{
	  v52 = (unsigned __int8)CInPacket::Decode1(iPacket);
	  v53 = (unsigned __int8)CInPacket::Decode1(iPacket);
	  --v51;
	  *(&aaDiscountRate + v53 + 30 * v52) = (unsigned __int8)CInPacket::Decode1(iPacket);
	}
	while ( v51 );
  }
  */
	write<signed char>(0); // size for something in CWvsContext::SetSaleInfo

	// -------------------------------------------------------------------------------

	// best items

	// 120 bytes
	/*for (int i = 0; i < 15; i++)
	{
		write<long long>(0);
	}*/

	/*
	00000054 m_aBest         CS_BEST 90 dup(?)

	00000000 CS_BEST         struc ; (sizeof=0xC, align=0x4, copyof_3068)
	00000000                                         ; XREF: CCashShop/r struc_1/r
	00000000 nCategory       dd ?
	00000004 nGender         dd ?
	00000008 nCommoditySN    dd ?
	0000000C CS_BEST         ends

	from some old packet sniffing, it seems that if there is no item to show, gender is -1/FF FF FF FF and commoditysn 0F 00 00 00

	or -1/FF FF FF FF for gender = both genders?

	  CInPacket::DecodeBuffer(iPacket, l->m_aBest, 0x438u);
  CCashShop::DecodeStock(v53, v54);
  CCashShop::DecodeLimitGoods(v53, v54);
  */

  /*for (int i = 1; i <= 8; i++)
  {
	  // for both genders
	  for (int j = 0; j < 2; j++)
	  {
		  write<int>(i); // nCategory
		  write<int>(j); // nGender
		  write<int>(50200004); // nCommoditySN

		  write<int>(i); // nCategory
		  write<int>(j); // nGender
		  write<int>(50200069); // nCommoditySN

		  write<int>(i); // nCategory
		  write<int>(j); // nGender
		  write<int>(50200008); // nCommoditySN

		  write<int>(i); // nCategory
		  write<int>(j); // nGender
		  write<int>(50100009); // nCommoditySN

		  write<int>(i); // nCategory
		  write<int>(j); // nGender
		  write<int>(50000047); // nCommoditySN
	  }
  }*/

  // 1080 bytes (-120) = 960 bytes, (-120 + -240) = 720 bytes
	unsigned int size = 0x438;
	write_null(size);

	// 240 bytes
	// "best item" 5 items
	// for eight categories?
	/*for (int i = 1; i <= 8; i++)
	{
		// for both genders
		for (int j = 0; j < 2; j++)
		{
			write<int>(i); // nCategory
			write<int>(j); // nGender
			write<int>(50200004); // nCommoditySN
			//
			write<int>(i);
			write<int>(j);
			write<int>(50200069);
			//
			write<int>(i);
			write<int>(j);
			write<int>(50200117);
			//
			write<int>(i);
			write<int>(j);
			write<int>(50100008);
			//
			write<int>(i);
			write<int>(j);
			write<int>(50000047);
		}
	}*/

	// -------------------------------------------------------------------------------
	// -------------------------------------------------------------------------------

	/*
	from v0.95 GMS

	int __thiscall CCashShop::DecodeStock(CCashShop *this, CInPacket *iPacket)
{
  CInPacket *v2; // ebx@1
  CCashShop *v3; // edi@1
  signed int v4; // esi@1
  int result; // eax@2
  ZArray<CS_STOCK> *v6; // edi@3

  v2 = iPacket;
  v3 = this;
  v4 = CInPacket::Decode2(iPacket);
  if ( v4 > 0 )
  {
	v6 = &v3->m_aStock;
	ZArray<CS_STOCK>::_Realloc(v6, v4, 1, (ZAllocHelper *)&iPacket);
	CInPacket::DecodeBuffer(v2, v6->a, 8 * v4);
	result = 1;
  }
  else
  {
	result = 0;
  }
  return result;
}

00000000 CS_STOCK        struc ; (sizeof=0x8, align=0x4, copyof_3066)
00000000 nSN             dd ?
00000004 nStockState     dd ?
00000008 CS_STOCK        ends
*/

	write<short>(0); // CCashShop::DecodeStock size

	/*
	from v0.95 GMS

	int __thiscall CCashShop::DecodeLimitGoods(CCashShop *this, CInPacket *iPacket)
{
  CInPacket *v2; // ebx@1
  CCashShop *v3; // edi@1
  signed int v4; // esi@1
  int result; // eax@2
  ZArray<CS_LIMITGOODS> *v6; // edi@3

  v2 = iPacket;
  v3 = this;
  v4 = CInPacket::Decode2(iPacket);
  if ( v4 > 0 )
  {
	v6 = &v3->m_aLimitGoods;
	ZArray<CS_LIMITGOODS>::_Realloc(v6, v4, 1, (ZAllocHelper *)&iPacket);
	CInPacket::DecodeBuffer(v2, v6->a, 104 * v4);
	result = 1;
  }
  else
  {
	result = 0;
  }
  return result;
}

00000000 CS_LIMITGOODS   struc ; (sizeof=0x68, align=0x4, copyof_1408)
00000000 nItemID         dd ?
00000004 nSN             dd 10 dup(?)
0000002C nState          dd ?
00000030 nOriginCount    dd ?
00000034 nRemainCount    dd ?
00000038 dwConditionFlag dd ?
0000003C nDateStart      dd ?
00000040 nDateEnd        dd ?
00000044 nHourStart      dd ?
00000048 nHourEnd        dd ?
0000004C abWeek          dd 7 dup(?)
00000068 CS_LIMITGOODS   ends

*/

	write<short>(0); // CCashShop::DecodeLimitGoods size

	write<bool>(false); // m_bEventOn
	write<int>(150); // m_nHighestCharacterLevelInThisAccount

	/*
	from v0.95 GMS
	at the end of the cashshop enter packet:

	CCashShop::LoadData(v2, iPacket);
	v2->m_bEventOn = (unsigned __int8)CInPacket::Decode1(v6);
	v2->m_nHighestCharacterLevelInThisAccount = CInPacket::Decode4(v6);
	*/
}

void PacketCreator::ShowCashPoints(int nx_credit)
{
	write<short>(send_headers::kSHOW_CASH);
	write<int>(nx_credit); // nx cash
	write<int>(0); // maple point
	write<int>(0); // prepaid
}

void PacketCreator::ShowBoughtCashItem(const std::shared_ptr<Item> &item, int account_id)
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<unsigned char>(0x3B); // action
	CashShopAddCashItemData(item, account_id);
}

void PacketCreator::IncreaseInventorySlots(signed char inventory_id, signed char slots)
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<signed char>(0x44); // action
	write<signed char>(inventory_id);
	write<short>(slots);
}

void PacketCreator::IncreaseStorageSlots(signed char slots)
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<unsigned char>(0x46); // action
	write<short>(slots);
}

void PacketCreator::GetCashShopInventory(std::vector<std::shared_ptr<Item>> items, int account_id, short storage_slots, short character_slots)
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<signed char>(0x2F); // action
	write<short>(static_cast<short>(items.size()));
	for (auto it : items)
	{
		CashShopAddCashItemData(it, account_id);
	}
	write<short>(storage_slots);
	write<short>(character_slots);
}

void PacketCreator::CashShopShowGifts()
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<signed char>(0x31); // action
	write<short>(0); // size
	// for each size write a little modified version of CashShopAddCashItemData
}

void PacketCreator::CashShopShowWishlist()
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<signed char>(0x33); // action
	for (int i = 0; i < 10; ++i)
	{
		write<int>(0); // item commodity SN
	}
}

void PacketCreator::TakeOutFromCashShopInventory(Item *item, short position)
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<unsigned char>(74); // action
	write<short>(position);
	ItemInfo(item, false);
}

void PacketCreator::TransferToCashShopInventory(const std::shared_ptr<Item> &item, int account_id)
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<unsigned char>(76); // action
	CashShopAddCashItemData(item, account_id);
}

/*

When declairing global variables, you can do the following

labelname db 0 ; define byte
labelname dw 0 ; define word
labelname dd 0  ; define double word
labelname dq 0 ; define quad word

from GMS v0.95 data:

CInPacket::DecodeBuffer(iPacket, v6, 0x37u);

0x37 = 55

void __thiscall CCashShop::OnCashItemResMoveStoLDone(CCashShop *this, CInPacket *iPacket)

DD = DWORD? DWORD = unsinged long = 4 bytes
DW = WORD? = unsigned short
_LARGE_INTEGER = struct of long and unsigned long = 4 + 4 bytes = 8 bytes

8
4
4
4
4
2
13
8
4
4

00000000 GW_CashItemInfo struc ; (sizeof=0x37, copyof_3069)
00000000 liSN            _LARGE_INTEGER ?
00000008 dwAccountID     dd ?
0000000C dwCharacterID   dd ?
00000010 nItemID         dd ?
00000014 nCommodityID    dd ?
00000018 nNumber         dw ?
0000001A sBuyCharacterID db 13 dup(?)
00000027 dateExpire      _FILETIME ?
0000002F nPaybackRate    dd ?
00000033 nDiscountRate   dd ?
00000037 GW_CashItemInfo ends
00000037
*/

void PacketCreator::CashShopAddCashItemData(const std::shared_ptr<Item> &item, int account_id)
{
	write<long long>(item->get_unique_id());
	write<int>(account_id);
	write<int>(0); // character id? dwCharacterID
	write<int>(item->get_item_id());
	write<int>(item->get_commodity_id_sn());
	write<short>(item->get_amount());
	write_string("", 13); // character name? (who made the gift?)
	write<long long>(item->get_expiration_time());
	write<int>(0); // nPaybackRate
	write<int>(0); // nDiscountRate
}
