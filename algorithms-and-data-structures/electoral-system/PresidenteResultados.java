package aed;

public class PresidenteResultados {
    int[] votos;
    int primeroId;
    int segundoId;
    int sumaTodosLosVotos;
    
    PresidenteResultados(int largoArray) {
        votos = new int[largoArray]; // O(1)
        segundoId = -1; // O(1)
        primeroId = -1; // O(1)
        sumaTodosLosVotos = 0; // O(1)
    }
}

