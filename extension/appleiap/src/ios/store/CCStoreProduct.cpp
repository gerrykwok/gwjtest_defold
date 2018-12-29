#if defined(DM_PLATFORM_IOS)

#include "store/CCStoreProduct.h"

StoreProduct* StoreProduct::productWithId(const char* productIdentifier,
                                              const char* localizedTitle,
                                              const char* localizedDescription,
                                              float price,
                                              const char* priceLocale)
{
    StoreProduct* product = new StoreProduct();
    product->initWithId(productIdentifier, localizedTitle, localizedDescription, price, priceLocale);
    product->autorelease();
    return product;
}

bool StoreProduct::initWithId(const char* productIdentifier,
                                const char* localizedTitle,
                                const char* localizedDescription,
                                float price,
                                const char* priceLocale)
{
    m_productIdentifier = productIdentifier ? productIdentifier : "";
    m_localizedTitle = localizedTitle ? localizedTitle : "";
    m_localizedDescription = localizedDescription ? localizedDescription : "";
    m_price = price;
    m_priceLocale = priceLocale ? priceLocale : "";
    return true;
}

#endif
