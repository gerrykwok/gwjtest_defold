#if defined(DM_PLATFORM_IOS)

#include "store/CCStorePaymentTransactionWrapper.h"
#import <StoreKit/StoreKit.h>

StorePaymentTransactionWrapper* StorePaymentTransactionWrapper::createWithTransaction(void* transactionObj)
{
    StorePaymentTransactionWrapper* transaction = new StorePaymentTransactionWrapper();
    transaction->m_transactionObj = transactionObj;
    [(SKPaymentTransaction *)transactionObj retain];
    return transaction;
}

StorePaymentTransactionWrapper::~StorePaymentTransactionWrapper(void)
{
    [(SKPaymentTransaction *)m_transactionObj release];
}

#endif
