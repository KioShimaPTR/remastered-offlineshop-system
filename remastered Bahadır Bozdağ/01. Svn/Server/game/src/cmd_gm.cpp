//Search:

	if (ch->GetShop())
		strlcat(buf, ", Shop", sizeof(buf));

//Add below:

#ifdef ENABLE_OFFLINE_SHOP_SYSTEM
	if (ch->GetOfflineShop())
		strlcat(buf, ", Offline Shop", sizeof(buf));
#endif