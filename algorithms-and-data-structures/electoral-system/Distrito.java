package aed;

public class Distrito {
    String nombre;
    int cantDeBancas;
    int ultimaMesa;
    int[] votosDiputados;
    int votosDiputadosTotal;

    Distrito(int cantPartidos, String nombre, int cantDeBancas, int ultimaMesa) {
        this.votosDiputados = new int[cantPartidos];
        this.nombre = nombre;
        this.cantDeBancas = cantDeBancas;
        this.ultimaMesa = ultimaMesa;
        this.votosDiputadosTotal = 0;
    }
}

