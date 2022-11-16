/*
    WiFiCentral.hpp - Código pertencente ao projeto da matéria de Oficinas de Integração 1
    Projeto - Central de Segurança Residencial
    Equipe 20 - Rafael Eijy Ishikawa Rasoto, Gabriel Spadafora e Nicolas Riuichi Oda.
*/

#pragma once

#include <Arduino.h>

void conectaWiFi();
void testeConexao(volatile char* statusWiFi);

void conectaTelegram();

bool enviaFormulario();

void handleRoot();
void handleNotFound();
void handleSubmit();

void mensagemTelegram();
void handleNewMessages(int numNewMessages);
void comandosTelegram(String texto, String nome);
void mensagemParaTelegram(String mensagem);
bool cadastroTelegram(bool tipoCadastro);
void acessoLiberadoWiFi(String nomeUsuario, int tipoEntrada);
void configHorario (String texto);
