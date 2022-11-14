/*
    ESPCAM.ino - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#include "ESPCAM.hpp"
#include "defineESPCAM.hpp"

/*************************************************************
Rotinas de Interrupção
*************************************************************/



/*************************************************************
Funções Principais
*************************************************************/

void setup()
{
    configuracao();
}

void loop()
{
    executar();
}
