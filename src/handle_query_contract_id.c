#include "staderbnb_plugin.h"

// Sets the first screen to display.
void handle_query_contract_id(void *parameters) {
    ethQueryContractID_t *msg = (ethQueryContractID_t *) parameters;
    const context_t *context = (const context_t *) msg->pluginContext;
    // msg->name will be the upper sentence displayed on the screen.
    // msg->version will be the lower sentence displayed on the screen.

    // For the first screen, display the plugin name.
    strlcpy(msg->name, "Staderlabs", msg->nameLength);
    char *msgVersion;

    // EDIT THIS: Adapt the cases by modifying the strings you pass to `strlcpy`.
    switch (context->selectorIndex) {
        case BSC_STAKEMANAGER_DEPOSIT:
            msgVersion = "Stake";
            break;

        case BSC_STAKEMANAGER_REQUEST_WITHDRAW:
            msgVersion = "Unstake";
            break;

        case BSC_STAKEMANAGER_CLAIM_WITHDRAW:
            msgVersion = "Claim";
            break;

        default:
            PRINTF("Selector index: %d not supported\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    strlcpy(msg->version, msgVersion, msg->versionLength);
    msg->result = ETH_PLUGIN_RESULT_OK;
}