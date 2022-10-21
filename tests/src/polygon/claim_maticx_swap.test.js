import "core-js/stable";
import "regenerator-runtime";
import {
  waitForAppScreen,
  zemu,
  nano_models,
  serializeTx,
} from "../test.fixture";

const contractAddr = "0xfd225c9e6601c9d38d8f98d8731bf59efcf8c0e3";
const pluginName = "staderlabs";
const testNetwork = "polygon";
const chainID = 137;
const signedPlugin = false;
// Test from replayed transaction: https://polygonscan.com/tx/0xe26214e3577874118e16ce1a605a9ad06b0836c0507e2071ddc3b4bac795b2a0
const inputData =
  "0x77baf2090000000000000000000000000000000000000000000000000000000000000000";

nano_models.forEach(function (model) {
  test(
    "[Nano " + model.letter + "] Claim MaticX Swap",
    zemu(
      model,
      async (sim, eth) => {
        const serializedTx = serializeTx(contractAddr, inputData, chainID);

        const tx = eth.signTransaction("44'/60'/0'/0/0", serializedTx);

        const right_clicks = model.letter === "S" ? 5 : 5;

        // Wait for the application to actually load and parse the transaction
        await waitForAppScreen(sim);
        // Navigate the display by pressing the right button `right_clicks` times, then pressing both buttons to accept the transaction.
        await sim.navigateAndCompareSnapshots(
          ".",
          testNetwork + "_maticx_" + model.name + "_claim_maticx_swap",
          [right_clicks, 0]
        );

        await tx;
      },
      signedPlugin,
      testNetwork
    )
  );
});
