# Aprende Lenguaje de Señas

Aplicación Android para apoyar el aprendizaje del alfabeto de la **Lengua de Señas Mexicana (LSM)** mediante la interacción con un guante robótico basado en **ESP32**. La app permite conectarse al guante por Bluetooth, visualizar la posición de la mano asociada a cada letra y enviar comandos para reproducirla físicamente.

## Funcionalidades principales

- Conexión con el guante por medio de Bluetooth.
- Controles para conectar o desconectar el guante.
- Alfabeto LSM completo de **A a Z**.
- Vista previa visual de la seña correspondiente a cada letra.

## Funcionamiento

1. El usuario empareja el guante ESP32 desde los ajustes Bluetooth de Android.
2. Desde la pantalla principal, la aplicación carga los dispositivos emparejados y permite seleccionar el guante.
3. La conexión se realiza mediante RFCOMM usando el UUID estándar de Serial Port Profile (SPP).
4. Al seleccionar una letra, la app muestra su representación visual y envía al ESP32 el carácter correspondiente seguido de un salto de línea.

## Tecnologías

- Kotlin
- Android SDK
- View Binding
- AndroidX
- Bluetooth Classic / RFCOMM
- Gradle Kotlin DSL

## Requisitos

- Android 6.0 (API 23) o superior.
- Dispositivo Android con Bluetooth.
- Guante o dispositivo ESP32 compatible con Bluetooth clásico SPP.
- El ESP32 debe estar emparejado previamente con el teléfono.
- Android Studio con un JDK compatible con la configuración de Gradle del proyecto.

## Compilación

Clona el repositorio y ábrelo directamente en Android Studio. Después de sincronizar Gradle, puedes compilarlo desde el IDE o ejecutar:

```bash
./gradlew assembleDebug
```

En Windows:

```powershell
gradlew.bat assembleDebug
```

## Protocolo de comandos

Los comandos enviados al ESP32 son caracteres ASCII en mayúscula seguidos de `\n`:

- `A` a `Z`: seleccionan una letra.
- `0`: coloca el guante en posición de reposo.

La aplicación mantiene una sola conexión activa y cierra el socket cuando el usuario se desconecta o se produce un error de comunicación.
