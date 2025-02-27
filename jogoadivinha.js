function iniciarJogo() {
    const numeroSecreto = Math.floor(Math.random() * 100) + 1;
    let tentativas = 0;

    console.log("Bem-vindo ao jogo Adivinha o Número!");
    console.log("Tenta adivinhar o número entre 1 e 100.");

    function adivinhar(numero) {
        tentativas++;
        if (numero === numeroSecreto) {
            console.log(`Parabéns! Adivinhaste o número ${numeroSecreto} em ${tentativas} tentativas.`);
        } else if (numero < numeroSecreto) {
            console.log("Muito baixo! Tenta outra vez.");
        } else {
            console.log("Muito alto! Tenta outra vez.");
        }
    }

    return adivinhar;
}

const jogo = iniciarJogo();


