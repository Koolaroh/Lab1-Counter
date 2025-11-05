#include "Vcounter.h"      // Include the model header (auto-generated later)
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"

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

    // Initialise Vbuddy
    if (vbdOpen() != 1) return (-1);     // Open connection; cfg path in vbuddy.cfg
    vbdHeader("Lab 1: Counter");         // Display a title on Vbuddy screen

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

        //Send counter value to Vbuddy 7-seg display
        vbdHex(4, (int(top->count) >> 16) & 0xF);
        vbdHex(3, (int(top->count) >> 8)  & 0xF);
        vbdHex(2, (int(top->count) >> 4)  & 0xF);
        vbdHex(1, (int(top->count) >> 0)  & 0xF);
        vbdCycle(i + 1);   // update Vbuddy screen

        // Change signals during simulation
        top->rst = (i < 2) | (i==15);              // Reset active for first 2 cycles
        top->en  = vbdFlag();   // Read enable from Vbuddy
        if (Verilated::gotFinish()) exit(0);
    }

    vbdClose();     // Close Vbuddy connection
    tfp->close();   // Close trace file
    exit(0);
}
