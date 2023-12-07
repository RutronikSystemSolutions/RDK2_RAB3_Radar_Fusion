/**
 * \addtogroup        Board
 * @{
 */

#ifndef BOARD_H
#define BOARD_H 1

/**
 * Each build project for a board has to implement this function
 * containing the necessary one-time initialization calls
 */
void Board_Constructor(void);

/**
 * Each build project for a board has to implement this function
 * containing the periodic calls which will be executed in the
 * bare-metal system's infinite loop
 */
void Board_run(void);

#endif /* BOARD_H */

/** @} */
