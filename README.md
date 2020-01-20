# Firmware řídího mikrokontroleru pro frekvenční syntezátor *MAX2871*
Cílem tohoto projektu je navrhnout hardware pro frekvenční syntezátor [**MAX2871**](https://www.maximintegrated.com/en/products/comms/wireless-rf/MAX2871.html) a ovládáním přes rozhraní USB. Pro převodník z USB na SPI rozhraní realizující komunikaci s frekvenčním syntezátorem má být použit mikrokontrolér řady STM32F0x.
## Obsah
* 1\. [Úvod](#Úvod)
* 2\. [Hardwarová část](#Hardwarová-část)
    * 1.1\. [Blokové schéma](#Blokové-schéma)
    * 1.2\. [Schéma zapojení](#schéma-zapojení)
    * 1.3\. [Návrh plošného spoje](#Návrh-plošného-spoje)
* 3\. [Firmware](#Firmware)

## Úvod
Kmitočtové syntezátory se používají pro generování harmonických průběhů s diskrétními kmitočty odvozených z jednoho (koherentní) či více zdrojů (nekoherentní). Tato práce se zabývá návrhem nepřímého kmitočtového syntezátoru s fázovým závěsem pro mikrovlnné kmitočty. Od navrhovaného modulu syntezátoru bude předpokládán dostatečný kmitočtový krok (alespoň 10 Hz), frekvenční stabilita a v neposlední řadě univerzálnost modulu. Proto byl zvolen obvod [**MAX2871**](https://www.maximintegrated.com/en/products/comms/wireless-rf/MAX2871.html), který je řízen přes rozhraní SPI.
## Hardwarová část
Deska plošného spoje byla navržena v programu KiCad. Celý projekt lze nalézt na tomto odkaze: [fiskusk/Synthesizer_HW_kicad_project_MAX2871](https://github.com/fiskusk/Synthesizer_HW_kicad_project_MAX2871)
### Blokové schéma
Blokové schéma navrženého modulu frekvenčního syntezátoru je na následujícím obrázku. 
<p align="center">
  <img src="/docu/images/main_block_diag.png" alt="Block diagram" width="600"/>
</p>

Napájení je umožněno buď z konektoru USB, nebo ze zdroje externího stejnosměrného napětí +5 V připojeného na k tomu určený konektor. Jako řídící mikrokontroler byl zvolen ***STM32F042F6P6***. Ten umožňuje komunikaci se syntezátorem skrze rozhraní USB, případně autonomní chod na základě pozice zkratovací propojky. Ta vybírá jedno ze 4 možných uložených nastavení pro syntezátor. Ty jsou po krátké prodlevě při změně pozice zkratovací propojky nahrány do syntezátoru.

Frekvenční syntezátor ***MAX2871*** obsahuje dva výstupy. Na jednom je umístěn výstupní zesilovač, který výkonově posílí generovaný signál ve frekvenčním pásmu od 23,5 MHz do 6 GHz. Na druhém výstupu je umístěna aktivní násobička kmitočtu dvěma, která umožňuje generovat výstupní kmitočet v rozsahu od 5 GHz do 12 GHz. Signál referenčního kmitočtu lze přepínat mezi interním teplotně kompenzovaným krystalovým oscilátorem (TCXO), nebo externím, který je přiveden na konektor SMA.

### Schéma zapojení
Na následujícím obrázku je výsledné kompletní schéma zapojení. Podrobnější rozbor zapojení lze nalézt v mé [semestrální práci.](docu/semestralni_diplomova_prace_Klapil.pdf)

![Schematic](/docu/images/Synthesizer_HW_full_schematic.png "Schéma zapojení modulu frekvečního syntezátoru")

### Návrh plošného spoje
Navržený prototyp desky plošného spoje musí mít vysokofrekvenční trasy impedančně přizpůsobeny 50 Ω. Pro dosažení co nejlepších parametrů by bylo vhodnější použít materiál dielektrika, který má na vyšších frekvencích menší ztráty. Nicméně s ohledem na jednodušší výrobu, byl pro první prototyp zvolen matriál dielektrika FR4 s relativní permitivitou ${\epsilon}_{r}$ = 4,4 a tloušťkou 0,8 mm. Šířka vysokofrekvenčního mikropáskového vedení byla stanovena za pomoci integrovaného kalkulátoru návrhového prostředí plošných spojů KiCad na 1,65 mm.

<p align="center">
  <img src="/docu/images/3d_vizualizace_s_popisky.png" alt="Block diagram" width="600" class="center"/>
</p>

## Firmware
Firmware byl vyvíjen za pomocí **HAL** knihoven v prostředí **Visual Studio Code**. Nastavení tohoto prostředí je uloženo v projektové složce [.vscode](.vscode/). Pro krokování programu je zapotřebí doinstalovat pár rozšíření. Především nástroj [Cortex-Debug](https://github.com/Marus/cortex-debug), [ARM toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads), [OpenOCD](https://github.com/ntfreak/openocd) a [CMSIS-SVD](https://github.com/posborne/cmsis-svd). Tyto informace lze také nalézt [zde](https://github.com/wykys/STM32-tools). Základ projektu byl pak vygenerován programem **STM32CubeMX**, ve kterém jsem si nechal vygenerovat i soubor [Makefile](Makefile). V tom bylo potřeba při vytvoření vlastních .c a .h je zahrnout do překladu.

### Úprava `Makefile`
```Makefile
C_SOURCES =  \
..
Src/max2871.c \
Src/flash.c \
Src/timer.c \
Src/format.c \
Src/usb.c \
```
Pro programování a ladění vyvíjeného programu jsem používal dev-kit [STM32F4DISCOVERY](https://www.st.com/en/evaluation-tools/stm32f4discovery.html), který obsahuje ST-LINK V2. Ten se dá jednoduše použít pro programování/ladění programu mimo vývojovou desku, po změně nastavení zkratovacích propojekt na kitu.

Program byl překládán utilitou make, kterou jsem měl nainstalovánou na Linuxovém sub-systému [(WSL)](https://docs.microsoft.com/en-us/windows/wsl/install-win10).


