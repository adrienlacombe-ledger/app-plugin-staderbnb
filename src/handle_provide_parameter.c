#include "staderbnb_plugin.h"

// Copy amount sent parameter to amount_received
static void handle_amount_received(const ethPluginProvideParameter_t *msg, context_t *context) {
    copy_parameter(context->amount_received, msg->parameter, sizeof(context->amount_received));
}

static void handle_unsupported_param(ethPluginProvideParameter_t *msg, const context_t *context) {
    PRINTF("Param not supported: %d\n", context->next_param);
    msg->result = ETH_PLUGIN_RESULT_ERROR;
}

static void handle_unstake(ethPluginProvideParameter_t *msg, context_t *context) {
    switch (context->next_param) {
        case UNSTAKE_AMOUNT:
            handle_amount_received(msg, context);
            context->next_param = UNEXPECTED_PARAMETER;
            break;
        // Keep this
        default:
            handle_unsupported_param(msg, context);
            break;
    }
}

void handle_provide_parameter(void *parameters) {
    ethPluginProvideParameter_t *msg = (ethPluginProvideParameter_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;
    // We use `%.*H`: it's a utility function to print bytes. You first give
    // the number of bytes you wish to print (in this case, `PARAMETER_LENGTH`) and then
    // the address (here `msg->parameter`).
    PRINTF("plugin provide parameter: offset %d\nBytes: %.*H\n",
           msg->parameterOffset,
           PARAMETER_LENGTH,
           msg->parameter);

    msg->result = ETH_PLUGIN_RESULT_OK;

    // EDIT THIS: adapt the cases and the names of the functions.
    switch (context->selectorIndex) {
        case BSC_STAKEMANAGER_REQUEST_WITHDRAW:
            handle_unstake(msg, context);
            break;

        case BSC_STAKEMANAGER_DEPOSIT:
        case BSC_STAKEMANAGER_CLAIM_WITHDRAW:
            context->next_param = UNEXPECTED_PARAMETER;
            return;

        default:
            PRINTF("Selector Index not supported: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}