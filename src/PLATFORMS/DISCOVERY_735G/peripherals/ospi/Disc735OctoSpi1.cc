#include "Common.h"
#include "Disc735OctoSpi1.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735OctoSpi1
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Disc735OctoSpi1::Disc735OctoSpi1 () :
  PhysicalOctoSpi (OCTO_SPI_1_REGISTERS),
  m_cs (GPIO_BANK_G, 6),
  m_d0 (GPIO_BANK_D, 11),
  m_d1 (GPIO_BANK_D, 12),
  m_d2 (GPIO_BANK_E, 2),
  m_d3 (GPIO_BANK_D, 13),
  m_d4 (GPIO_BANK_D, 4),
  m_d5 (GPIO_BANK_D, 5),
  m_d6 (GPIO_BANK_G, 9),
  m_d7 (GPIO_BANK_D, 7),
  m_dqs (GPIO_BANK_B, 2),
  m_clk (GPIO_BANK_F, 10)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735OctoSpi1
// @method:   activateGpio
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Disc735OctoSpi1::activateGpio ()
{
  m_cs.activate (10, GPIO_SPEED_HIGH, GPIO_LOAD_PULL_UP, GPIO_OUTPUT_PUSH_PULL);
  m_d0.activate (9, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_d1.activate (9, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_d2.activate (9, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_d3.activate (9, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_d4.activate (10, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_d5.activate (10, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_d6.activate (9, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_d7.activate (10, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_dqs.activate (10, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_clk.activate (9, GPIO_SPEED_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
}

