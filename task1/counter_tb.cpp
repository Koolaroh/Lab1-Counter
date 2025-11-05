#include "Vcounter.h"      // Include the model header (auto-generated later)
#include "verilated.h"
#include "verilated_vcd_c.h"

int main(int argc, char **argv, char **env) {
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);   // Initialise Verilator args

    // Create instance of top module (Device Under Test)
    Vcounter* top = new Vcounter;

    // Enable waveform tracing
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("counter.vcd");

    // Initialise signals
    top->clk = 1;
    top->rst = 1;
    top->en  = 0;

    // Run simulation for 300 clock cycles
    for (i = 0; i < 300; i++) {
        // Each iteration = one clock cycle (two toggles)
        for (clk = 0; clk < 2; clk++) {
            tfp->dump(2*i + clk);        // Record waveform
            top->clk = !top->clk;        // Toggle clock
            top->eval();                 // Evaluate model
        }

        // Change signals during simulation
        top->rst = (i < 2);              // Reset active for first 2 cycles
        top->en  = (i > 4);              // Enable counter after cycle 4

        if (Verilated::gotFinish()) exit(0);
    }

    tfp->close();
    exit(0);
}
