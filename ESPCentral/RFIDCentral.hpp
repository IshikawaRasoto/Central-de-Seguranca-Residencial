/*
    RFIDCentral.hpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>

extern bool flagCadastro;

class Usuarios {
    public:
        String Nome;
        String ID;
        String Sair;
        bool flagSair;
        String Entrar;
        bool flagEntrar;

        Usuarios(String ID = "");
        ~Usuarios();
};

void configuraRFID_SD();
void verificaRFID();
void verificaTXT();
void verificaTAG(String tipo);
void salvaTAG(const int x);
void cadastraTAG();
void cadastraTAG_SD();
void jaCadastrado();
void ativaRFID();
void ativaSD();
void acessoLiberado();
void acessoNegado();
void setCadastro(const bool x);
void listaCadastro();
void salvaNomeCadastro(String nome);
void listaTelegram ();
void deletaUsuario(String texto);
void configuraVetorUsuarios();
