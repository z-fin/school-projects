package aed;

public class Partido {
    int id;
    int votosDiputadosDistrito;
    int cociente;
    int divisiones;

    public Partido(int id, int votosDiputadosDistrito) {
        this.id = id; // O(1)
        this.votosDiputadosDistrito = votosDiputadosDistrito; // O(1)
        this.cociente = votosDiputadosDistrito; // O(1)
        this.divisiones = 1; // O(1)
    }


}
