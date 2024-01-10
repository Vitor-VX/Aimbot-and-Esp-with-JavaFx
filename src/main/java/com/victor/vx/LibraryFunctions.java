package com.victor.vx;

import com.sun.jna.Library;
import com.sun.jna.Native;

public interface LibraryFunctions extends Library {    
    LibraryFunctions INSTANCE = Native.load("C:\\Users\\Cliente\\OneDrive\\Área de Trabalho\\projeto-java\\bin\\Aimbot.dll", LibraryFunctions.class);

    void ModFunctions();

    // stop's
    boolean StopHacking();
    boolean StopAimbotAutomatic();
    boolean StopAimbotFromShot();
    boolean StopNoRecoil();
    boolean StopBalletInfity();
    boolean EspLinhaActivate();

    // esp
    boolean EspType(int valor);
}