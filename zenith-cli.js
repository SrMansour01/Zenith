#!/usr/bin/env node
const { program } = require('commander');
const { spawn } = require('child_process');  // Mudamos para spawn

program
  .command("run")
  .option("--mobile", "Inicia o Expo")
  .action((options) => {
    if (options.mobile) {
      const expoProcess = spawn("npx", ["expo", "start"], {
        cwd: "./mobile",  // Muda o diretório
        stdio: "inherit"  // Mostra a saída diretamente no terminal
      });

      expoProcess.on("error", (error) => {
        console.error("Erro ao iniciar o Expo:", error);
      });
    }
  });

program.parse(process.argv);