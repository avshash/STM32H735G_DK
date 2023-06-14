#include "Common.h"
#include "Disc735Ltdc.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735Ltdc
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Disc735Ltdc::Disc735Ltdc () :
  PhysicalLtdc (),
  m_clk (GPIO_BANK_G, 7),
  m_vsync (GPIO_BANK_A, 4),
  m_hsync (GPIO_BANK_C, 6),
  m_r0 (GPIO_BANK_E, 0),
  m_r1 (GPIO_BANK_H, 3),
  m_r2 (GPIO_BANK_H, 8),
  m_r3 (GPIO_BANK_H, 9),
  m_r4 (GPIO_BANK_H, 10),
  m_r5 (GPIO_BANK_H, 11),
  m_r6 (GPIO_BANK_E, 1),
  m_r7 (GPIO_BANK_E, 15),
  m_g0 (GPIO_BANK_B, 1),
  m_g1 (GPIO_BANK_B, 0),
  m_g2 (GPIO_BANK_A, 6),
  m_g3 (GPIO_BANK_E, 11),
  m_g4 (GPIO_BANK_H, 15),
  m_g5 (GPIO_BANK_H, 4),
  m_g6 (GPIO_BANK_C, 7), 
  m_g7 (GPIO_BANK_D, 3),
  m_b0 (GPIO_BANK_G, 14),
  m_b1 (GPIO_BANK_D, 0),
  m_b2 (GPIO_BANK_D, 6),
  m_b3 (GPIO_BANK_A, 8),
  m_b4 (GPIO_BANK_E, 12),
  m_b5 (GPIO_BANK_A, 3),
  m_b6 (GPIO_BANK_B, 8), 
  m_b7 (GPIO_BANK_B, 9),
  m_de (GPIO_BANK_E, 13),
  m_disp (GPIO_BANK_D, 10),
  m_bl_ctrl (GPIO_BANK_G, 15)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735Ltdc
// @method:   activateGpio
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Disc735Ltdc::activateGpio ()
{
  m_clk.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_vsync.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_hsync.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_r0.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_r1.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_r2.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_r3.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_r4.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_r5.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_r6.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_r7.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_g0.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_g1.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_g2.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_g3.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_g4.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_g5.activate (9, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_g6.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_g7.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_b0.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_b1.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_b2.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_b3.activate (13, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_b4.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_b5.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_b6.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_b7.activate (14, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);

  m_de.activatePushPull (GPIO_SPEED_LOW, false);
  m_disp.activatePushPull (GPIO_SPEED_LOW, true);
  m_bl_ctrl.activatePushPull (GPIO_SPEED_LOW, true);
}

