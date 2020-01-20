# Firmware řídího mikrokontroleru pro frekvenční syntezátor *MAX2871*
Cílem tohoto projektu je navrhnout hardware pro frekvenční syntezátor [**MAX2871**](https://www.maximintegrated.com/en/products/comms/wireless-rf/MAX2871.html) a ovládání přes rozhraní USB. Pro převodník z USB na SPI rozhraní realizující komunikaci s frekvenčním syntezátorem má být použit mikrokontrolér řady STM32F0x.
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
Firmware byl vyvíjen za pomocí **HAL** knihoven v prostředí **Visual Studio Code**. Nastavení tohoto prostředí je uloženo v projektové složce [.vscode](.vscode/). Pro krokování programu je zapotřebí doinstalovat pár rozšíření. Především nástroj [Cortex-Debug](https://github.com/Marus/cortex-debug), [ARM toolchain](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads), [OpenOCD](https://github.com/ntfreak/openocd) a [CMSIS-SVD](https://github.com/posborne/cmsis-svd). Tyto informace lze také nalézt [zde](https://github.com/wykys/STM32-tools). Základ projektu byl pak vygenerován programem **STM32CubeMX**, ve kterém jsem si nechal vygenerovat i soubor [Makefile](Makefile). V něm bylo potřeba zahrnout do překladu vytvořené vlastní .c a .h soubory.

Úprava `Makefile`
```make
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

### Hlavní program
Hlavní program před vstupem do nekonečné smyčky provede inicializaci periferií s využitím HAL knihoven. Dále spustí časovač 3, provede část z úkonů pro přesměrování výstupu stdout na USB VCP (například pro příkaz `printf`), počká 100 ms a spustí moji inicializační rutinu `init_routine()`. V nekonečné smyčce pak volá `running_routine()`, která se věnuje obsluze hlavního programu.

Inicializační rutina má za úkol při zjištění, že uživatelskí paměť pro 4 nastavení syntezátoru je prázdná, nahrát defaultní hodnoty a provést inicializaci syntezátoru s respektováním vybrané pozice zkratovací propojkou.

```C
void init_routine(void)
{
    if (saved_data_1[0] == 0x0)
    {
        load_default_memory_register_values();
    }
    apply_memory_select_changed(PLO_INIT);
}
```

Funkce volaná v nekonečné smyčce vykonává obsluhu stavů, podle toho, zda-li je sériový port otevřen, či nikoliv. 

Pokud je port otevřený, pro testovací účely si rozvěcuji led diodu. Jestliže jsou k dispozici nová data v kruhovém zásobníku, který je plněn při změně stavu zavěšení smyčky fázového závěsu PLO, postupně je zpracovávám. Zpracováním je myšleno odeslání odpovídajícího textového řetězce na sériovou linku. Jako poslední se volá funkce, zajišťující zpracování přijatých řídících dat z rozhraní USB.

Jestliže je port zavřený, opět využívám led diody pro indikaci zavřeného portu (dioda nesvítí) a kontroluji stav, jestli nebyla provedena změna na zkratovací propojce, která určuje nastavený PLO.

```C
void running_routine(void)
{
    switch (host_com_port_open_closed)
    {
    case HOST_COM_PORT_OPENED:
        PLO_MODULE_OUT2_ON; // TODO for test purpose only

        // check if new plo lock status occur and eventually process them
        uint8_t data;
        while (plo_buff_pop(&data))
        {
            plo_process_lock_status((bool)data);
        }
        // check if new usb command is ready and eventually process them.
        usb_procesing_command_data();
        break;
    case HOST_COM_PORT_CLOSED:
        PLO_MODULE_OUT2_OFF; // TODO for test purpose only
        // if jumper possition has changed, load new setting into plo
        if (memory_select_event == MEMORY_SELECT_CHANGED)
            apply_memory_select_changed(PLO_NEW_DATA);
        break;
    default:
        break;
    }
}
```

### Autonomní režim řízení syntezátoru
Navržený modul je schopný si uchovat i po odpojení napájení 4 nastavení syntezátoru, mezi kterými lze vybírat vybírat zkratovací propojkou. Ta je umístěna na kratší hraně modulu u konektoru USB. Pro uchování nastavení i při odpojeném napájení, je nutné data uložit do paměti programu. 

#### Datová paměť v oblasti paměti programu
Funkce, které zajišťují práci s programovou pamětí se nacházejí v souboru [flash.c](Src/flash.c). Dále bylo nutné upravit linker script soubor [STM32F042F6Px_FLASH.ld](STM32F042F6Px_FLASH.ld).

V něm jsem nadefinoval novou oblast, kterou jsem pojmenoval `DATA`. Ta je umístěna na konci programové paměti, čímž jsem flash paměť rozdělil na dva sektory o velikosti 31 kB pro program a 1 kB pro data.

```
MEMORY
{
RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 6K
FLASH (rx)      : ORIGIN = 0x8000000, LENGTH = 31K
DATA (xrw)      : ORIGIN = 0x08007C00, LENGTH = 1K
}
```

Následně je ještě nutné tuto oblast definovat v `SECTIONS{...}` přidáním následujícího kódu:

```
...
user_data :
{
    . = ALIGN(4);
        *(.user_data)
    . = ALIGN(4);
} > DATA
...
```

Proměnné uchovávající nastavení lze pak kódu definovat takto:
```
__attribute__((__section__(".user_data"))) uint32_t saved_data_1[7];
__attribute__((__section__(".user_data"))) uint32_t saved_data_2[7];
__attribute__((__section__(".user_data"))) uint32_t saved_data_3[7];
__attribute__((__section__(".user_data"))) uint32_t saved_data_4[7];
```

7 pozic pro každou ze 4 pamětí. Šest 32 bitových neznaménkových celých číslech (pozice 0-6) uchovává  informaci o všech šesti registrech pro PLO MAX2871. Sedmá pozice pak slouží pro uložení nastavení modulu syntezátoru. Tedy o tom, jaký výstup je aktivní, jestli byla nastavena interní nebo externí signálová reference. Případně je dostatek dalších bitů pro pamatování dalších možností, jako třeba rozmítání kmitočtu, které je zamýšleno doprogramovat. 0. bit uchovává informaci o prvním výstupu (1 aktivní, 0 neaktivní), 1. bit o druhém (1 akt., 0 neakt.) a 2. bit o signálové referenci (1 externí, 0 interní).

Do paměti programu nelze zasahovat, dokud není odemčena HAL funkcí `HAL_FLASH_Unlock()`. Po provedení patřičných změn je zase nutné přístup k paměti uzamknout, k čemu slouží funkce `HAL_FLASH_Lock()`.

Dalším nutným úkonem, bez kterého nelze data do této paměti zapisovat, je vymazání celé stránky dat, kam se data budou zapisovat. Existuje HAL funkce, která by toto měla zajišťovat, nicméně mě tato funkce nechtěla plnit svůj účel. Proto jsem [na tomto odkaze](https://community.st.com/s/question/0D50X00009XkfIO/stm32f0-help-with-flash-to-read-and-write-hal-libraries) našel rešení a napsal vlastní funkci pro smazání stránky v programové paměti. Do funkce vstupuje adresa začátku stránky. Paměť je rozdělena, jak již bylo několikrát zmíněno na stránky, kdy každá stránka má 1kB. Viz [tento dokument](https://www.st.com/content/ccc/resource/technical/document/reference_manual/c2/f8/8a/f2/18/e6/43/96/DM00031936.pdf/files/DM00031936.pdf/jcr:content/translations/en.DM00031936.pdf) na stránce 55.

```C
void myFLASH_PageErase(uint32_t address)
{
    HAL_FLASH_Unlock();     // first of all, you must unlock access to flash 
    FLASH->CR |= FLASH_CR_PER;                  /* (1) */
    FLASH->AR = address;                        /* (2) */
    FLASH->CR |= FLASH_CR_STRT;                 /* (3) */
    while ((FLASH->SR & FLASH_SR_BSY) != 0)     /* (4) */
    if ((FLASH->SR & FLASH_SR_EOP) != 0)        /* (5) */
    {
        FLASH->SR |= FLASH_SR_EOP;              /* (6)*/
    }
    FLASH->CR &= ~FLASH_CR_PER;                 /* (7) */
    HAL_FLASH_Lock();       // finnaly lock the flash memory
}
```
Samotný zápis uživatelských dat pak využívá HAL funkce `HAL_StatusTypeDef HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data)`. Funkci volám s parametrem  `TypeProgram` v mém případě `FLASH_TYPEPROGRAM_WORD`, čímž definuji, že budu zapisovat celé 32. bitové číslo. Adresa, kam se má zapisovat bude například pro první paměť `(&saved_data_1[index])`, kde index představuje o jaký registr se jedná (0-6). Posledním parametrem už jsou samotná 32-bitová `Data`.

Pro jednoduší implementaci v mé aplikaci jsem napsal funkci `void write_data_to_flash(uint8_t position, uint8_t index, uint32_t data)`, která na pozici (1-4) ukládá data určená pro jeden ze 7 registrů (index = 0 až 6).

```
void write_data_to_flash(uint8_t position, uint8_t index, uint32_t data)
{
    HAL_FLASH_Unlock();     // First of all unlock flash partition access

    switch (position)
    {
    case 1:
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)(&saved_data_1[index]), data);
        break;
    case 2:
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)(&saved_data_2[index]), data);
        break;
    case 3:
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)(&saved_data_3[index]), data);
        break;
    case 4:
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)(&saved_data_4[index]), data);
        break;
    }
    HAL_FLASH_Lock();       // lock flash partition
}
```

Jelikož, je někdy potřeba zapsat obsah všech 7 registrů pro určitou pozici v paměti naráz vytvořil jsem funkci `void write_complete_data_to_flash(uint8_t possition, char *val0, char *val1, char *val2, char *val3, char *val4, char *val5, char *val6)`. Tedy na pozici 1-4 uloží data, která do funkce stupují jako *valX (kde x je registr 0-6).

Jednou z funkcí, která pracuje s těmito uživatelskými daty je `void apply_memory_select_changed(plo_new_data_t plo_write_type)`. Ta je volána například při inicializaci modulu syntezátoru po spuštění, nebo při změně pozice zkratovací propojky. Podle toho také pak data nahraje do syntezátoru MAX2871. Při přivedení napájení je totiž nutné provést inicializační proceduru, která bude dále popsána v sekci týkající se funkcí pro PLO MAX2871. Ta je totiž odlišná od algoritmu změny řídících registrů při normálním provozu. Při parametru `plo_write_type == PLO_INIT` funkce nečeká, než se nastaví `tick_handle` na `TICK_OCCUR` (jelikož je z hlavního programu volána v nekonečné smyčce) a okamžitě nahráje data z uživatelské paměti do frekvenčního syntezátoru jako inicializační. Během dalšího volání v běžném provozu se funkce volá s parametrem PLO_NEW_DATA, kdy nahrávání do PLO je podmínkou pozdrženo, než se dosáhne periody čítače (max 1 s), kdy se nastaví TICK_OCCUR (před voláním funkce je nastaven na TICK_NOT_OCCUR). Tímto jsou ošetřeny případné zákmity signálu při změně pozice propojky. Předpoklad je, že po takovém čase už je signál na pinu po změně stabilní.

```
void apply_memory_select_changed(plo_new_data_t plo_write_type)
{
    if ((tick_handle == TICK_OCCUR) || (plo_write_type == PLO_INIT))
    {
        // Get jumper settings and switch-case apply changes
        switch (((1 << 0) & ~HAL_GPIO_ReadPin(JP2_GPIO_Port, JP2_Pin)) | \
            (((1 << 0) & ~HAL_GPIO_ReadPin(JP1_GPIO_Port, JP1_Pin)) << 1))
        {
        case 0:
            plo_write(saved_data_1, plo_write_type);
            change_plo_module_states(saved_data_1[6]);
            break;
        case 1:
            plo_write(saved_data_2, plo_write_type);
            change_plo_module_states(saved_data_2[6]);
            break;
        case 2:
            plo_write(saved_data_3, plo_write_type);
            change_plo_module_states(saved_data_3[6]);
            break;
        case 3:
            plo_write(saved_data_4, plo_write_type);
            change_plo_module_states(saved_data_4[6]);
            break;
        default:
            plo_write(saved_data_1, plo_write_type);
            change_plo_module_states(saved_data_1[6]);
            break;
        }
        memory_select_event = MEMORY_SELECT_WAIT;
    }
}
```
Pro každou pozici zkratovací propojky, je po nahrání registrů do MAX2871 také aplikováno nastavení pro komponenty modulu syntezátoru (aktivace odpovídajících výstupů, volba reference). K tomu slouží právě funkce `void change_plo_module_states(uint32_t control_register)`.

```
void change_plo_module_states(uint32_t control_register)
{
    (control_register & (1 << 0)) ? PLO_MODULE_OUT1_ON : PLO_MODULE_OUT1_OFF;
    ((control_register & (1 << 1)) >> 1) ? PLO_MODULE_OUT2_ON : PLO_MODULE_OUT2_OFF;
    ((control_register & (1 << 2)) >> 2) ? PLO_MODULE_EXT_REF : PLO_MODULE_INT_REF;
}
```
Poslední nepopsanou funkcí, která s touto pamětí operuje, je funkce `void flash_send_stored_data(void)`. Ta jednoduše odešle obsah uživatelské paměti na USB VCP.

#### Změny pozice zkratovací propojky
Při generování projektu v CubeMX jsem nastavil volání přerušení v případě, že dojde ke změně signálu na pinech, kde je umístěna pinová lišta pro zkratovací propojku. Přerušení se vygeneruje jak pro sestupnou tak i pro náběžnou hranu. Obsluha přerušení se nachází ve funkci `void EXTI4_15_IRQHandler(void)` v souboru [stm32f0xx_it.c](Src/stm32f0xx_it.c), který je již předpřipravený generováním kódu z CubeMX. Přerušení musí být krátké, proto se v pouze nastaví příznak, že byla detekována změna `memory_select_event = MEMORY_SELECT_CHANGED` a vyresetuje se `tick_handle = TICK_NOT_OCCUR`. Což jak už bylo zmíněno, slouží k aplikaci neblokujícího čekání pro ošetření před zákmity. Tyto příznaky se pak kontrolují v hlavním programu, viz. [výše](#hlavní-program).

```
void EXTI4_15_IRQHandler(void)
{
    /* USER CODE BEGIN EXTI4_15_IRQn 0 */
    memory_select_event = MEMORY_SELECT_CHANGED; //jumper memory select changed handle
    tick_handle = TICK_NOT_OCCUR;                // reset tick handle
    /* USER CODE END EXTI4_15_IRQn 0 */
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_7);
    /* USER CODE BEGIN EXTI4_15_IRQn 1 */

    /* USER CODE END EXTI4_15_IRQn 1 */
}
```

#### Použití časovače
Použitý časovač číslo 3 slouží pouze pro generování příznaku, že uplynula jistá doba. Čítač byl s pomocí CubeMX nastaven s periodou 500 ms a pro znásobení času využívám podmínky s počítáním, kolikrát byla perioda zavolána. Nyní si takto generuji tick o délce 1 sekundy. Toto použití mám nachystáno pro případné možné rozšíření, kdy si takto mohu generovat více příznaků s různou délkou. Když změním periodu čítače na například 10 ms, mohu si třeba počítat přerušení od periody čítače s různými násobkami základní délky s lepším rozlišením. Což mohu stejně jako doposud využívat pro neblokující čekání. Je dobré mít na vědomí, že je rozdíl mezi přerušením od samotného časovače, kdy je volána rutina v souboru [stm32f0xx_it.c](Src/stm32f0xx_it.c) a přerušení od periody, která je násobkem. Tato rutina se nachází v souboru [timer.c](src/timer.c). Tato funkce je HAL knihovnách definovaná jak `weak`, tedy zde se přepíše mou vlastní implementací.

```
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)
    {
        /**< Counting the number of times an interrupt from the timer occurred */
        static uint8_t cnt = 0;
        // this is period interupt, which occur each 500ms
        if (cnt > 1)
        {   // so this occur every 1 second
            tick_handle = TICK_OCCUR;
            cnt = 0;
        }
        else
        {
            cnt++;
            tick_handle = TICK_NOT_OCCUR;
        }
    }
}
```

### USB komunikace





