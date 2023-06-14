#include "Common.h"
#include "Disc735OctoSpi2.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735OctoSpi2
// @method:   constructor
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Disc735OctoSpi2::Disc735OctoSpi2 () :
  PhysicalOctoSpi (OCTO_SPI_2_REGISTERS),
  m_cs (GPIO_BANK_G, 12),
  m_d0 (GPIO_BANK_F, 0),
  m_d1 (GPIO_BANK_F, 1),
  m_d2 (GPIO_BANK_F, 2),
  m_d3 (GPIO_BANK_F, 3),
  m_d4 (GPIO_BANK_G, 0),
  m_d5 (GPIO_BANK_G, 1),
  m_d6 (GPIO_BANK_G, 10),
  m_d7 (GPIO_BANK_G, 11),
  m_dqs (GPIO_BANK_F, 12),
  m_clk (GPIO_BANK_F, 4)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// @class:    Disc735OctoSpi2
// @method:   activateGpio
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void
Disc735OctoSpi2::activateGpio ()
{
  m_cs.activate (3, GPIO_SPEED_VERY_HIGH, GPIO_LOAD_PULL_UP, GPIO_OUTPUT_PUSH_PULL);
  m_d0.activate (9, GPIO_SPEED_VERY_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_d1.activate (9, GPIO_SPEED_VERY_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_d2.activate (9, GPIO_SPEED_VERY_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_d3.activate (9, GPIO_SPEED_VERY_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_d4.activate (9, GPIO_SPEED_VERY_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_d5.activate (9, GPIO_SPEED_VERY_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_d6.activate (3, GPIO_SPEED_VERY_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_d7.activate (9, GPIO_SPEED_VERY_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_dqs.activate (9, GPIO_SPEED_VERY_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
  m_clk.activate (9, GPIO_SPEED_VERY_HIGH, GPIO_LOAD_DISCONNECTED, GPIO_OUTPUT_PUSH_PULL);
}

