/* terminal.h - interface de terminal do kernel */
#ifndef TERMINAL_H
#define TERMINAL_H

/* Inicializa o terminal */
void terminal_init(void);

/* Loop principal do terminal (nunca retorna) */
void terminal_run(void);

#endif /* TERMINAL_H */
