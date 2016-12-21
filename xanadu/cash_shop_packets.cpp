//

#include "packetcreator.hpp"

#include "player.hpp"
#include "send_packet_opcodes.hpp"

void PacketCreator::EnterCashShop(Player *player)
{
	write<short>(send_headers::kOPEN_CASHSHOP);
	writeCharacterData(player);
	write<signed char>(1);
	write<std::string>("test"); // username?

	/*
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
	write<int>(0); // probably: size for something in CWvsContext::SetSaleInfo


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
	write<short>(0); // probably: size for something in CWvsContext::SetSaleInfo

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
	write<signed char>(0); // probably: size for something in CWvsContext::SetSaleInfo

	// -------------------------------------------------------------------------------

	// not sure if this one is in the right place
	// 120 bytes
	for (int i = 0; i < 15; i++)
	{
		write<long long>(0);
	}
	
	// not sure if this one is in the right place
	// 240 bytes
	for (int i = 1; i <= 8; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			write<int>(i);
			write<int>(j);
			write<int>(50200004);
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
	}

	write<short>(0); // probably CCashShop::DecodeStock size

	write<short>(0); // probably CCashShop::DecodeLimitGoods size

	write<short>(0); // probably DecodeZeroGoods size

	write<signed char>(0); // probably m_bEventOn
	write<int>(75); // probably m_nHighestCharacterLevelInThisAccount

	/*
	from v0.95 GMS
	at the end of the cashshop enter packet:
	CCashShop::LoadData(v2, iPacket);
	v2->m_bEventOn = (unsigned __int8)CInPacket::Decode1(v6);
	v2->m_nHighestCharacterLevelInThisAccount = CInPacket::Decode4(v6);

	from at the end of cashshop loaddata:

	CCashShop::DecodeZeroGoods(v53, v54);
	v79 = -1;
	if ( pBase.m_pInterface )
	pBase.m_pInterface->vfptr->Release((IUnknown *)pBase.m_pInterface);
	}

	int __thiscall CCashShop::DecodeZeroGoods(CCashShop *this, CInPacket *iPacket)
	{
	CInPacket *v2; // ebx@1
	CCashShop *v3; // edi@1
	signed int v4; // esi@1
	int result; // eax@2
	ZArray<CS_ZEROGOODS> *v6; // edi@3

	v2 = iPacket;
	v3 = this;
	v4 = CInPacket::Decode2(iPacket);
	if ( v4 > 0 )
	{
	v6 = &v3->m_aZeroGoods;
	ZArray<CS_ZEROGOODS>::_Realloc(v6, v4, 1, (ZAllocHelper *)&iPacket);
	CInPacket::DecodeBuffer(v2, v6->a, 68 * v4);
	result = 1;
	}
	else
	{
	result = 0;
	}
	return result;
	}
	*/
}

void PacketCreator::ShowCashPoints(int nx_credit)
{
	write<short>(send_headers::kSHOW_CASH);
	write<int>(nx_credit); // nx cash (credit)
	write<int>(0); // maple point
	write<int>(0); // nx cash (others)
}

void PacketCreator::ShowBoughtCashItem(int account_id, int serial_number, int item_id, int amount)
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<unsigned char>(0xFE); // action
	write<short>(0);
	write<long long>(1); // cash unique id
	write<int>(account_id);
	write<int>(0);
	write<int>(item_id);
	write<int>(serial_number);
	write<short>(static_cast<short>(amount));
	write_string("", 13);
	write<long long>(0); // expiration time
	write<long long>(0);
}

void PacketCreator::IncreaseInventorySlots(signed char inventory_id, signed char slots)
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<signed char>(127); // action
	write<signed char>(inventory_id);
	write<signed char>(slots);
	write<signed char>(0);
}

void PacketCreator::IncreaseStorageSlots(signed char slots)
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<unsigned char>(129); // action
	write<signed char>(slots);
	write<signed char>(0);
}

void PacketCreator::GetCashShopInventory(short storage_slots, short character_slots)
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<signed char>(0x4B); // action
	short size = 0;
	write<short>(size); // cashshop inventory size
	// TODO: write items here that are in the cashshop inventory
	if (size > 0)
	{
		write<int>(0);
		// todo: write all info
	}
	write<short>(storage_slots);
	write<short>(character_slots);
}

void PacketCreator::TakeOutFromCashShopInventory(Item *item, short position)
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<unsigned char>(0x83); // action
	write<short>(position);
	ItemInfo(item, false);
	write<int>(0);
}

void PacketCreator::TransferToCashShopInventory(int account_id, int serial_number, int item_id, int amount)
{
	write<short>(send_headers::kCASHSHOP_OPERATION);
	write<unsigned char>(0x85); // action
	write<long long>(1); // cash unique id
	write<int>(account_id);
	write<int>(0);
	write<int>(item_id);
	write<int>(serial_number);
	write<short>(static_cast<short>(amount));
	write_bytes("000000000000000000000000007016AFFF60BCCE010000000000000000D28C8E010000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
}
