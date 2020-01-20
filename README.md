# Firmware řídího mikrokontroleru pro frekvenční syntezátor *MAX2871*
Cílem tohoto projektu je navrhnout hardware pro frekvenční syntezátor [**MAX2871**](https://www.maximintegrated.com/en/products/comms/wireless-rf/MAX2871.html) a ovládání přes rozhraní USB. Pro převodník z USB na SPI rozhraní realizující komunikaci s frekvenčním syntezátorem má být použit mikrokontrolér řady STM32F0x.
## Obsah
* 1\. [Úvod](#1.-Úvod)
* 2\. [Hardwarová část](#2.-Hardwarová-část)
  * 2.1\. [Blokové schéma](#2.1-Blokové-schéma)
  * 2.2\. [Schéma zapojení](#2.2-Schéma-zapojení)
  * 2.3\. [Návrh plošného spoje](#2.3-Návrh-plošného-spoje)
* 3\. [Firmware](#3.-Firmware)
  * 3.1\. [Hlavní program](#3.1-Hlavní-program)
  * 3.2\. [Autonomní režim řízení syntezátoru](#3.2-Autonomní-režim-řízení-syntezátoru)
    * 3.2.1\. [Datová paměť v oblasti paměti programu](#3.2.1-Datová-paměť-v-oblasti-paměti-programu)
    * 3.2.2\. [Změny pozice zkratovací propojky](#3.2.2-Změny-pozice-zkratovací-propojky)
    * 3.2.3\. [Použití časovače](#3.2.3-Použití-časovače)
  * 3.3\. [USB komunikace (VCP)](#3.3-USB-komunikace-(VCP))
    * 3.3.1\. [Odesílání příkazů přes sériové rozhraní](#3.3.1-Odesílání-příkazů-přes-sériové-rozhraní)
    * 3.3.2\. [Příjem dat ze sériového rozhraní](#3.3.2-Příjem-dat-ze-sériového-rozhraní)
    * 3.3.3\. [Zpracování přijatých příkazů](#3.3.3-Zpracování-přijatých-příkazů)
  * 3.4\. [Ovládání frekvenčního syntezátoru MAX2871](#3.4-Ovládání-frekvenčního-syntezátoru-MAX2871)
  * 3.5\. [Indikace zavěšení smyčky fázového závěsu PLO](#3.5-Indikace-zavěšení-smyčky-fázového-závěsu-PLO)
* 4.\. [Video demonstrující funkci](4.-Video-demonstrující-funkci)
* 5.\. [Závěr](5.-Závěr)
* 6.\. [Zdroje](6.-Zdroje)

## 4. Video demonstrující funkci

## 5. Závěr

## 6. Zdroje

## 1. Úvod
Kmitočtové syntezátory se používají pro generování harmonických průběhů s diskrétními kmitočty odvozených z jednoho (koherentní) či více zdrojů (nekoherentní). Tato práce se zabývá návrhem nepřímého kmitočtového syntezátoru s fázovým závěsem pro mikrovlnné kmitočty. Od navrhovaného modulu syntezátoru bude předpokládán dostatečný kmitočtový krok (alespoň 10 Hz), frekvenční stabilita a v neposlední řadě univerzálnost modulu. Proto byl zvolen obvod [**MAX2871**](https://www.maximintegrated.com/en/products/comms/wireless-rf/MAX2871.html), který je řízen přes rozhraní SPI.
## 2. Hardwarová část
Deska plošného spoje byla navržena v programu KiCad. Celý projekt lze nalézt na tomto odkaze: [fiskusk/Synthesizer_HW_kicad_project_MAX2871](https://github.com/fiskusk/Synthesizer_HW_kicad_project_MAX2871)
### 2.1 Blokové schéma
Blokové schéma navrženého modulu frekvenčního syntezátoru je na následujícím obrázku. 
<p align="center">
  <img src="/docu/images/main_block_diag.png" alt="Block diagram" width="600"/>
</p>

Napájení je umožněno buď z konektoru USB, nebo ze zdroje externího stejnosměrného napětí +5 V připojeného na k tomu určený konektor. Jako řídící mikrokontroler byl zvolen ***STM32F042F6P6***. Ten umožňuje komunikaci se syntezátorem skrze rozhraní USB, případně autonomní chod na základě pozice zkratovací propojky. Ta vybírá jedno ze 4 možných uložených nastavení pro syntezátor. Ty jsou po krátké prodlevě při změně pozice zkratovací propojky nahrány do syntezátoru.

Frekvenční syntezátor ***MAX2871*** obsahuje dva výstupy. Na jednom je umístěn výstupní zesilovač, který výkonově posílí generovaný signál ve frekvenčním pásmu od 23,5 MHz do 6 GHz. Na druhém výstupu je umístěna aktivní násobička kmitočtu dvěma, která umožňuje generovat výstupní kmitočet v rozsahu od 5 GHz do 12 GHz. Signál referenčního kmitočtu lze přepínat mezi interním teplotně kompenzovaným krystalovým oscilátorem (TCXO), nebo externím, který je přiveden na konektor SMA.

### 2.2 Schéma zapojení
Na následujícím obrázku je výsledné kompletní schéma zapojení. Podrobnější rozbor zapojení lze nalézt v mé [semestrální práci.](docu/semestralni_diplomova_prace_Klapil.pdf)

![Schematic](/docu/images/Synthesizer_HW_full_schematic.png "Schéma zapojení modulu frekvečního syntezátoru")

### 2.3 Návrh plošného spoje
Navržený prototyp desky plošného spoje musí mít vysokofrekvenční trasy impedančně přizpůsobeny 50 Ω. Pro dosažení co nejlepších parametrů by bylo vhodnější použít materiál dielektrika, který má na vyšších frekvencích menší ztráty. Nicméně s ohledem na jednodušší výrobu, byl pro první prototyp zvolen matriál dielektrika FR4 s relativní permitivitou ${\epsilon}_{r}$ = 4,4 a tloušťkou 0,8 mm. Šířka vysokofrekvenčního mikropáskového vedení byla stanovena za pomoci integrovaného kalkulátoru návrhového prostředí plošných spojů KiCad na 1,65 mm.

<p align="center">
  <img src="/docu/images/3d_vizualizace_s_popisky.png" alt="Block diagram" width="600" class="center"/>
</p>

## 3. Firmware
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
Pro programování a ladění vyvíjeného programu jsem používal dev-kit [STM32F4DISCOVERY](https://www.st.com/en/evaluation-tools/stm32f4discovery.html), který obsahuje ST-LINK V2. Ten se dá jednoduše použít pro programování/ladění programu mimo vývojovou desku, po změně nastavení zkratovacích propojek na kitu.

Program byl překládán utilitou make, která je nainstalována na Linuxovém sub-systému [(WSL)](https://docs.microsoft.com/en-us/windows/wsl/install-win10).

### 3.1 Hlavní program
Hlavní program před vstupem do nekonečné smyčky provede inicializaci periferií s využitím HAL knihoven. Dále spustí časovač 3, provede část z úkonů pro přesměrování výstupu stdout na USB VCP (například pro příkaz `printf`), počká 100 ms a spustí moji inicializační rutinu `init_routine()`. V nekonečné smyčce pak volá `running_routine()`, která se věnuje obsluze hlavního programu.

Inicializační rutina má za úkol při zjištění, že uživatelská paměť pro 4 nastavení syntezátoru je prázdná, nahrát defaultní hodnoty a provést inicializaci syntezátoru s respektováním vybrané pozice zkratovací propojkou.

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

Pokud je port otevřený, pro testovací účely si rozsvěcuji led diodu. Jestliže jsou k dispozici nová data v kruhovém zásobníku, který je plněn při změně stavu zavěšení smyčky fázového závěsu PLO, postupně je zpracovávám. Zpracováním je myšleno odeslání odpovídajícího textového řetězce na sériovou linku. Jako poslední se volá funkce, zajišťující zpracování přijatých řídících dat z rozhraní USB.

Jestliže je port zavřený, opět využívám led diody pro indikaci zavřeného portu (dioda nesvítí) a kontroluji stav, jestli nebyla provedena změna na zkratovací propojce, která určuje nastavení PLO.

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

### 3.2 Autonomní režim řízení syntezátoru
Navržený modul je schopný si uchovat i po odpojení napájení 4 nastavení syntezátoru, mezi kterými lze vybírat zkratovací propojkou. Ta je umístěna na kratší hraně modulu u konektoru USB. Pro uchování nastavení i při odpojeném napájení, je nutné data uložit do paměti programu. 

#### 3.2.1 Datová paměť v oblasti paměti programu
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

Proměnné uchovávající nastavení lze pak v kódu definovat takto:
```C
__attribute__((__section__(".user_data"))) uint32_t saved_data_1[7];
__attribute__((__section__(".user_data"))) uint32_t saved_data_2[7];
__attribute__((__section__(".user_data"))) uint32_t saved_data_3[7];
__attribute__((__section__(".user_data"))) uint32_t saved_data_4[7];
```

7 pozic pro každou ze 4 pamětí. Šest 32 bitových neznaménkových celých číslech (pozice 0-6) uchovává  informaci o všech šesti registrech pro PLO MAX2871. Sedmá pozice pak slouží pro uložení nastavení modulu syntezátoru. Tedy o tom, jaký výstup je aktivní, jestli byla nastavena interní nebo externí signálová reference. Případně je dostatek dalších bitů pro pamatování dalších možností, jako třeba rozmítání kmitočtu, které je zamýšleno doprogramovat. 0. bit uchovává informaci o prvním výstupu (1 aktivní, 0 neaktivní), 1. bit o druhém (1 akt., 0 neakt.) a 2. bit o signálové referenci (1 externí, 0 interní).

Do paměti programu nelze zasahovat, dokud není odemčena HAL funkcí `HAL_FLASH_Unlock()`. Po provedení patřičných změn je zase nutné přístup k paměti uzamknout, k čemu slouží funkce `HAL_FLASH_Lock()`.

Dalším nutným úkonem, bez kterého nelze data do této paměti zapisovat, je vymazání celé stránky dat, kam se data budou zapisovat. Funkce pro zápis totiž nic nezapíše, dokud není obsah paměťového místa roven `0xFFFFFFFF`. Vymazáním se právě tohoto stavu docílí. Toto je tedy nutné provést i při přepisování jedné pozice paměti. Existuje HAL funkce, která by toto měla zajišťovat. Nicméně tato funkce má nějaké chyby a nefungovala mi. Proto jsem [na tomto odkaze](https://community.st.com/s/question/0D50X00009XkfIO/stm32f0-help-with-flash-to-read-and-write-hal-libraries) našel rešení a napsal vlastní funkci pro smazání stránky v programové paměti. Do funkce vstupuje adresa začátku stránky. Funkce tedy smaže celou stránku se všemi čtyrmi soubory dat. Proto je nutné při zápisu z počítače odeslat zpět kompletní obsah pro všechny čtyři soubory dat. 

Paměť je rozdělena, jak již bylo několikrát zmíněno na stránky, kdy každá stránka má 1kB. Viz [tento dokument](https://www.st.com/content/ccc/resource/technical/document/reference_manual/c2/f8/8a/f2/18/e6/43/96/DM00031936.pdf/files/DM00031936.pdf/jcr:content/translations/en.DM00031936.pdf) na stránce 55.

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

Pro jednodušší implementaci v mé aplikaci jsem napsal funkci `void write_data_to_flash(uint8_t position, uint8_t index, uint32_t data)`, která na pozici (1-4) ukládá data určená pro jeden ze 7 registrů (index = 0 až 6).

```C
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

Jelikož je někdy potřeba zapsat obsah všech 7 registrů pro určitou pozici v paměti naráz, vytvořil jsem funkci `void write_complete_data_to_flash(uint8_t possition, char *val0, char *val1, char *val2, char *val3, char *val4, char *val5, char *val6)`. Tedy na pozici 1-4 uloží data, která do funkce stupují jako *valX (kde x je registr 0-6).

Jednou z funkcí, která pracuje s těmito uživatelskými daty je `void apply_memory_select_changed(plo_new_data_t plo_write_type)`. Ta je volána například při inicializaci modulu syntezátoru po spuštění, nebo při změně pozice zkratovací propojky. Podle toho také pak data nahraje do syntezátoru MAX2871. Při přivedení napájení je totiž nutné provést inicializační proceduru, která bude dále popsána v sekci týkající se funkcí pro PLO MAX2871. Ta je totiž odlišná od algoritmu změny řídících registrů při normálním provozu. Při parametru `plo_write_type == PLO_INIT` funkce nečeká, než se nastaví `tick_handle` na `TICK_OCCUR` (jelikož je z hlavního programu volána v nekonečné smyčce) a okamžitě data nahraje z uživatelské paměti do frekvenčního syntezátoru jako inicializační. Během dalšího volání v běžném provozu se funkce volá s parametrem PLO_NEW_DATA, kdy nahrávání do PLO je podmínkou pozdrženo, než se dosáhne periody čítače (max 1 s), kdy se nastaví TICK_OCCUR (před voláním funkce je nastaven na TICK_NOT_OCCUR). Tímto jsou ošetřeny případné zákmity signálu při změně pozice propojky. Předpokladem je, že po takovém čase už je signál na pinu po změně stabilní.

```C
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

```C
void change_plo_module_states(uint32_t control_register)
{
    (control_register & (1 << 0)) ? PLO_MODULE_OUT1_ON : PLO_MODULE_OUT1_OFF;
    ((control_register & (1 << 1)) >> 1) ? PLO_MODULE_OUT2_ON : PLO_MODULE_OUT2_OFF;
    ((control_register & (1 << 2)) >> 2) ? PLO_MODULE_EXT_REF : PLO_MODULE_INT_REF;
}
```
Poslední nepopsanou funkcí, která s touto pamětí operuje, je funkce `void flash_send_stored_data(void)`. Ta jednoduše odešle obsah uživatelské paměti na USB VCP.

#### 3.2.2 Změny pozice zkratovací propojky
Při generování projektu v CubeMX jsem nastavil volání přerušení v případě, že dojde ke změně signálu na pinech, kde je umístěna pinová lišta pro zkratovací propojku. Přerušení se vygeneruje jak pro sestupnou tak i pro náběžnou hranu. Obsluha přerušení se nachází ve funkci `void EXTI4_15_IRQHandler(void)` v souboru [stm32f0xx_it.c](Src/stm32f0xx_it.c), který je již předpřipravený generováním kódu z CubeMX. Přerušení musí být krátké, proto se pouze nastaví příznak, že byla detekována změna `memory_select_event = MEMORY_SELECT_CHANGED` a vyresetuje se `tick_handle = TICK_NOT_OCCUR`. Což jak už bylo zmíněno, slouží k aplikaci neblokujícího čekání pro ošetření před zákmity. Tyto příznaky se pak kontrolují v hlavním programu, viz. [výše](#hlavní-program).

```C
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

#### 3.2.3 Použití časovače
Použitý časovač číslo 3 slouží pouze pro generování příznaku, že uplynula jistá doba. Čítač byl s pomocí CubeMX nastaven s periodou 500 ms a pro znásobení času využívám podmínky s počítáním, kolikrát byla perioda zavolána. Nyní si takto generuji tick o délce 1 sekundy. Toto použití mám nachystáno pro případné možné rozšíření, kdy si takto mohu generovat více příznaků s různou délkou. Když změním periodu čítače například na 10 ms, mohu si třeba počítat přerušení od periody čítače s různými násobkami základní délky s lepším rozlišením. Což mohu stejně jako doposud využívat pro neblokující čekání. Je dobré mít na vědomí, že je rozdíl mezi přerušením od samotného časovače, kdy je volána rutina v souboru [stm32f0xx_it.c](Src/stm32f0xx_it.c) a přerušení od periody, která je násobkem. Tato rutina se nachází v souboru [timer.c](src/timer.c). Tato funkce je HAL knihovnách definovaná jak `weak`, tedy zde se přepíše mou vlastní implementací.

```C
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

### 3.3 USB komunikace (VCP)
Pro komunikaci je použito rozhraní USB v režimu virtuálního sériového portu. (Odtud zkratka VCP - Virtual Com Port). Knihovna byla vygenerována v programu CubeMX jako middleware. Při otevření portu terminál operačního systému Windows používá CDC příkazy pro nastavení kódování linky a pak vyčítá toto nastavení zpět. Více informací lze nalézt na tomto [odkaze](https://blog.brichacek.net/wp-content/uploads/2015/10/STM32F4-and-USB.pdf). Tyto příkazy jsou zachyceny ve funkci `static int8_t CDC_Control_FS(uint8_t cmd, uint8_t *pbuf, uint16_t length)`. Příkaz je detekován jako `cmd == CDC_SET_LINE_CODING` a `cmd == CDC_GET_LINE_CODING`. Tato funkce je také volána s příkazem `cmd == CDC_SET_CONTROL_LINE_STATE` v momentě, kdy se port otevře, případně zavře. Čehož využívám dále v kódu a stav portu si zde zachycuji. 
```C
...
uint8_t buffer[7];
...
static int8_t CDC_Control_FS(uint8_t cmd, uint8_t *pbuf, uint16_t length)
{
    ...
    case CDC_SET_LINE_CODING:
        // previous stored line coding from computer (CDC_GET_LINE_CODING) load into pbuf
        buffer[0] = pbuf[0];
        buffer[1] = pbuf[1];
        buffer[2] = pbuf[2];
        buffer[3] = pbuf[3];
        buffer[4] = pbuf[4];
        buffer[5] = pbuf[5];
        buffer[6] = pbuf[6];
        break;

    case CDC_GET_LINE_CODING:
        // this get line coding from computer after port open
        pbuf[0] = buffer[0];
        pbuf[1] = buffer[1];
        pbuf[2] = buffer[2];
        pbuf[3] = buffer[3];
        pbuf[4] = buffer[4];
        pbuf[5] = buffer[5];
        pbuf[6] = buffer[6];
        break;

    case CDC_SET_CONTROL_LINE_STATE:
        // this is used for get line state, if port opened or closed
        req = (USBD_SetupReqTypedef *)pbuf;
        if ((req->wValue & 0x0001) != 0)
        {
            host_com_port_open_closed = HOST_COM_PORT_OPENED;
        }
        else
        {
            memory_select_event = MEMORY_SELECT_CHANGED;
            host_com_port_open_closed = HOST_COM_PORT_CLOSED;
        }
        break;
    ...
}
```

Samotné ovládání modulu syntezátoru je realizováno za pomocí jednoduchých textových příkazů.

#### 3.3.1 Odesílání příkazů přes sériové rozhraní
Už v [hlavním programu](#hlavní-program) byla zavolána funkce `setbuf(stdout, NULL)` Tu volám proto, abych pro odesílání dat na sériovou linku mohl využívat funkci formátovaného výstupu `printf()`. Pro dokončení přesměrování výstupu `stdout` na USB VCP je nutno využít následujícího kódu, který se v mém programu nachází v [usbd_cdc_if.c](src/usbd_cdc_if.c) ke konci v sekci `/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */`

```C
int _write(int file, char const *buf, int n)
{
    // block in this endless while loop, if USB is busy
    while (((USBD_CDC_HandleTypeDef *)(hUsbDeviceFS.pClassData))->TxState != 0){}
    CDC_Transmit_FS((uint8_t *)(buf), n);
    return n;
}
```
Funkce v nekonečné smyčce počká, pokud bude stav linky BUSY. Po uvolnění linky zavolá funci pro odesílání dat na USB.

Příkazy jsou odesílány v následující funkci:
```C
uint8_t CDC_Transmit_FS(uint8_t *Buf, uint16_t Len)
{
    uint8_t result = USBD_OK;
    /* USER CODE BEGIN 7 */
    USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
    result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
    /* USER CODE END 7 */
    return result;
}
```

#### 3.3.2 Příjem dat ze sériového rozhraní
Po naplnění přijímacího bufferu se zavolá následující funkce v souboru [usbd_cdc_if.c](src/usbd_cdc_if.c):
```C
static int8_t CDC_Receive_FS(uint8_t *Buf, uint32_t *Len)
{
    /* USER CODE BEGIN 6 */
    // going through the received buffer character by character
    for (uint8_t i = 0; i < *Len; i++)
    {
        usb_data_available(Buf[i]);
    }

    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
    USBD_CDC_ReceivePacket(&hUsbDeviceFS);

    return (USBD_OK);
    /* USER CODE END 6 */
}
```
Ta po jednotlivých znacích volá funkci `void usb_data_available(uint8_t c)` ze souboru [usb.c](Src/usb.c). V ní jsou jednotlivé znaky ukládány do příkazového bufferu `cmd_buffer`. Jednotlivé příkazy mohou být odděleny znakem '\n' (LF) nebo '\r' (CR). Takto se rozpozná, že příkaz je kompletní, na jeho konec se zapíše znak `NULL` a nastaví se příznak, že buffer obsahuje kompletní ovládací instrukci. Nádledně dojde k přepnutí příkazového bufferu na druhý.
```C
void usb_data_available(uint8_t c)
{
    static uint8_t active_buff = 0;     /**< Handle actual active buffer    */
    uint8_t *pos = &cmd_buffer[active_buff].length;

    if (cmd_buffer[active_buff].received)
        return; // Buffer not free, cannot receive data...

    if (c == '\n' || c == '\r')
    {
        cmd_buffer[active_buff].buffer[*pos] = 0; // Add ending zero
        cmd_buffer[active_buff].received = 1;     // Mark data in buffer as received
        if (++active_buff >= CMD_BUFFER_CNT)      // Switch to next buffer
            active_buff = 0;                      // No more buffers, switch to first.
    }
    else
    {
        if (*pos < (CMD_BUFFER_LEN - 1)) // 1 Byte on the end is reserved for zero
        {
            cmd_buffer[active_buff].buffer[*pos] = c; // Save character to buffer
            *pos = *pos + 1;
        }
        else
        {
            //TODO: No more space in buffer, cannot store data.
            *pos = *pos; // Useless, just for filling in the "else" branch
        }
    }
}
```
Příkazy se tedy ukládají do více bufferů, mezi kterými program přepíná. Počet bufferů pro příkazy, které bude mít mikrokontroler k dispozici definuje makro `CMD_BUFFER_CNT`. Maximální počet znaků v bufferu určije makro `CMD_BUFFER_LEN`. Není ošetřen stav, kdy budou všechny zásobníky plné. Tedy příkazy z rozhraní USB budou chodit rychleji, než bude mikrokontroler schopný zpracovat. Do tohoto stavu by se totiž program teoreticky neměl dostat. Jak bude blíže rozebráno v sekci zpracování příkazů, řídící program v počítači bude po každém příkazu čekat na potvrzení zpracování dat.
#### 3.3.3 Zpracování přijatých příkazů
Pokud je port otevřený, zjišťuje se v nekonečné smyčce voláním funkce `void usb_procesing_command_data(void)` v [hlavním programu](#hlavní-program), zda-li nebyl přijat nový příkaz. Pokud ano, obsah zásobníku je předán funkci `void usb_process_command(char *command_data)`, která přijatý příkaz zpracuje. Po zpracování příkazu je příznak, že zásobník obsahuje nový příkaz zrušen a je přepnuto na další zásobník. Opět je kontrolováno, zda-li v něm nenachází nový. Pokud ano, postup se opakuje. Pokud ne, funkce zde končí.

Funkce zpracování všech příkazů je poměrně dlouhá, proto zde uvedu jen její část, na kterém vysvětlím princip rozdělení přijatého textového řetězce na jednotlivé příkazy.

Jako první si deklaruji pomocné proměnné, do kterých budu postupně ukládat rozdělený text:
```C
char *command;      /**< Recieved command               */
char *sub_command;  /**< Sub-command                    */
char *value;        /**< Carry command action value     */
char *register0;       /**< Carry register 0 value         */
char *register1;       /**< Carry register 1 value         */
char *register2;       /**< Carry register 2 value         */
char *register3;       /**< Carry register 3 value         */
char *register4;       /**< Carry register 4 value         */
char *register5;       /**< Carry register 5 value         */
char *module_control;       /**< Carry module controls value    */
```
Následně odstraním bílé znaky CR a LF, které jsou použity pro oddělení jednotlivých kompletních příkazů při příjmu.
```C
for (uint8_t i = 0; i < strlen(command_data); i++)
{
    command_data[i] = (command_data[i] < 32 || command_data[i] > 126) ? '\0' : command_data[i];
}
```
Postup rozdělování lze demonstrovat na příkazu pro výběr referenčního zdroje. 
```C
command = strtok(command_data, " ");    // Command part parse 
// reference selection part
if (strcasecmp(command, "ref") == 0)
{
    value = strtok(NULL, " ");          // Action part parse
    if (strcasecmp(value, "ext") == 0)
    {
        PLO_MODULE_EXT_REF;
    }

    else if (strcasecmp(value, "int") == 0)
    {
        PLO_MODULE_INT_REF;
    }
    printf("OK\r");                     // Sends a confirmation text string
}
```
Pro oddělení prvního příkazu využívám funkce s parametry `strtok(command_data, " ")`. Prvním argumentem je zpracovávaný textový řetězec a druhým je oddělovací znak. Získaný příkaz je pak uložen do pomocné proměnné command. Takto jsem získal první příkaz. Ten v podmínce testuji, zda-li se shoduje s nějakým definovaným řetězcem, například `"ref"`. Druhý příkaz se dostane voláním stejné funkce, ovšem s následujícími parametry `strtok(NULL, " ")`. Opakovým voláním funkce s těmito parametry se pak dostávají další a další příkazy oddělené v tomto případě mezerou (pokud jsou k dispozici). Po aplikování příkazu se odešle zpět do počítače text `"OK\r"`. Pokud jsou příkazy odesílány počítačovým programem, měl by na tento řetězec počkat, než bude pokračovat ve vysílání dalších příkazů. Na jiný textový řetězec modul syntezátoru odpoví `unknown command!`.

Některé příkazy zpracovávají hexadecimální výraz v textové podobě. Pro další zpracování, například uložení do programové paměti, nebo odeslání do PLO musí být tento řetězec převeden na číslo. K tomu slouží funkce `uint32_t hex2int(char *hex)` v souboru [format.c](Src/format.c). Její algoritmus byl nalezen [zde](https://stackoverflow.com/questions/10156409/convert-hex-string-char-to-int/39394256#39394256). Do funkce tedy vstupuje textový řetězec čísla v hexadecimální podobě a funkce vrátí jeho převedenou 32 bitovou hodnotu. Maximální převáděná hodnota tedy může být 8 hexadecimálních znaků / 32 bitů. 
```C
uint32_t hex2int(char *hex)
{
    uint32_t val = 0;
    while (*hex)
    {
        // get current character then increment
        uint8_t byte = *hex++;
        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9')
            byte = byte - '0';
        else if (byte >= 'a' && byte <= 'f')
            byte = byte - 'a' + 10;
        else if (byte >= 'A' && byte <= 'F')
            byte = byte - 'A' + 10;
        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        val = (val << 4) | (byte & 0xF);
    }
    return val;
}
```
Celá struktura všech možných příkazů, na který modul reaguje v tomto momentě je v následujícím seznamu:

|             **Příkaz**              |                                     **Popis**                                     | 
|:-----------------------------------:|:----------------------------------------------------------------------------------|
|             `ref ext`               | přepne na externí zdroj referenčního signálu                                      |
|             `ref int`               | přepne na interní zdroj referenčního signálu                                      |
|             `out 1 on`              | aktivuje výstupní zesilovač na 1. výstupu PLO                                     |
|            `out 1 off`              | deaktivuje výstupní zesilovač na 1. výstupu PLO                                   |
|             `out 2 on`              | aktivuje aktivní násobičku dvěma na 2. výstupu PLO                                |
|            `out 2 off`              | deaktivuje aktivní násobičku dvěma na 2. výstupu PLO                              |
|             `plo init`              | provede inicializační algoritmus nahrání testovacích dat do PLO                   |
|     `plo set_register 12345678`     | odešle 8 hexadecimálních znaků do PLO (32. bit. registr)                          |
|          `plo data clean`           | smaže všechna uživatelská data  uložená v programové paměti                       |
|         `plo data stored?`          | vrátí všechna uživatelská data  uložená v programové paměti                       |
|  `plo data 1 R0 R1 R2 R3 R4 R5 RC`  | nahraje data pro 1. paměť. R0-5 jsou reg. MAX2871, RC je registr modulu, viz [zde](#Autonomní-režim-řízení-syntezátoru) |
|  `plo data 2 R0 R1 R2 R3 R4 R5 RC`  | nahraje data pro 1. paměť. R0-5 jsou reg. MAX2871, RC je registr modulu, viz [zde](#Autonomní-režim-řízení-syntezátoru) |
|  `plo data 3 R0 R1 R2 R3 R4 R5 RC`  | nahraje data pro 1. paměť. R0-5 jsou reg. MAX2871, RC je registr modulu, viz [zde](#Autonomní-režim-řízení-syntezátoru) |
|  `plo data 4 R0 R1 R2 R3 R4 R5 RC`  | nahraje data pro 1. paměť. R0-5 jsou reg. MAX2871, RC je registr modulu, viz [zde](#Autonomní-režim-řízení-syntezátoru) |

### 3.4 Ovládání frekvenčního syntezátoru MAX2871
Funkce, které zajišťují přímo komunikaci obvodem frekvenčního syntezátoru lze nalézt v souboru [max2871.c](Src/max2871.c). Použití pinů mikrokontroleru, které mají jako alternativní funkci možnost SPI rozhraní by příliš zkomplikovalo návrh designu plošného spoje. Z tohoto důvodu bylo rozhodnuto realizovat komunikaci s PLO softwarovou implementací SPI rozhraní. K tomuto účelu slouží funkce `void plo_write_register(uint32_t data)`. Na jejím samotném začátku se musí zajistit, aby byla data do syntezátoru nahrávána od nejvíce významného bitu (MSB) po nejméně významný (LSB). Za tímto účelem se volá funkce `uint32_t lsb_to_msb_bit_reversal(uint32_t input)` ze souboru [format.c](Src/format.c), kde jsem se inspiroval [zde](https://stackoverflow.com/questions/7467997/reversing-the-bits-in-an-integer-x).

```C
uint32_t lsb_to_msb_bit_reversal(uint32_t input)
{
    input = (((input & 0xaaaaaaaa) >> 1) | ((input & 0x55555555) << 1));
    input = (((input & 0xcccccccc) >> 2) | ((input & 0x33333333) << 2));
    input = (((input & 0xf0f0f0f0) >> 4) | ((input & 0x0f0f0f0f) << 4));
    input = (((input & 0xff00ff00) >> 8) | ((input & 0x00ff00ff) << 8));
    return ((input >> 16) | (input << 16));
}
```

Podle vstupních dat je v cyklu bit po bitu nastavován datový výstup a generován hodinový pulz. Po odeslání všech 32 bitů je před ukončením funkce generován pulz. Ten má za funkci, že se nahraná data v posuvném registru v PLO aplikují.

```C
void plo_write_register(uint32_t data)
{
    // first reverse bits input number LSB->MSB
    data = lsb_to_msb_bit_reversal(data);
    // In the cycle, it passes through the individual bits of the input number
    // and sets the data output pin accordingly. 
    // It generates a clock pulse in each cycle.
    for (uint8_t j = 0; j < 32; j++)
    {
        (data & 0x01) ? PLO_DATA_SET : PLO_DATA_RESET;

        // generate clock pulse and shift next
        PLO_CLK_SET;
        data >>= 1;
        PLO_CLK_RESET;
    }
    // generate update pulse
    PLO_LE_SET;
    PLO_LE_RESET;
    // get down data wire
    PLO_DATA_RESET;
}
```

Jelikož je potřeba odlišit nahrávání dat při prvním spuštění syntezátoru a při běžném provozu, vznikly následující dvě funkce. `void plo_write_all(uint32_t *max2871, plo_new_data_t plo_write_type)` (ta upraví vstupní data v případě, že se jedná o inicializační algoritmus) a `void plo_write(uint32_t *data, plo_new_data_t plo_new_data_type)`. Data jsou vždy nahrávána od posledního po nultý registr. Samotný inicializační algoritmus spočívá v nahrátí kompletní posloupnosti všech šesti registrů s deaktivovaným výstupem samotného PLO (pátý a šestý bit čtvrtého registru je nastaven do logické nuly), počkání 20mms, znovu nahrání všech registrů s deaktivovaným výstupem PLO. Posledním krokem je aktivování výstupu, tedy nahráním originálního obsahu 4tého registru a ukončení funkce. Při běžném režimu se nahrají všechny registry beze změny.

```C
void plo_write_all(uint32_t *max2871, plo_new_data_t plo_write_type)
{
    for (int8_t i = 5; i >= 0; i--)
    {
        if (plo_write_type == PLO_OUT_ENABLE)
            i = 4;

        uint32_t c = max2871[i];

        if ((plo_write_type == PLO_INIT) && (i == 4))
            c &= ~((1UL << 5) | (1UL << 9));

        plo_write_register(c);

        if ((plo_write_type == PLO_INIT) && (i == 5))
            HAL_Delay(20);

        if (plo_write_type == PLO_OUT_ENABLE)
            i = -1;
    }
}
```

```C
void plo_write(uint32_t *data, plo_new_data_t plo_new_data_type)
{
    if (plo_new_data_type == PLO_INIT)
    {
        plo_write_all(data, PLO_INIT);
        plo_write_all(data, PLO_INIT);
        plo_write_all(data, PLO_OUT_ENABLE);
    }
    else if (plo_new_data_type == PLO_NEW_DATA)
    {
        plo_write_all(data, PLO_NEW_DATA);
    }
}
```

### 3.5 Indikace zavěšení smyčky fázového závěsu PLO
Frekvenční syntezátor MAX2871 obsahuje pin MUXOUT, který představuje víceúčelový pin. Jednou z funkcí je digitální detekce zavěšení smyčky fázového závěsu. Pokud jsou do příslušného registru PLO nahrána data, která nenastavují tuto funkci MUXOUT pinu, je odeslán textový řetězec `"plo state is not known\r"`. V momentě, kdy má PLO funkci MUXOUT pinu nastavenou správně a sériový port je otevřený, změna úrovně na pinu mikroprocesoru v přerušení ukládá do kruhového bufferu informaci, jaká úroveň při přerušení byla. Ta se pak v nekonečné smyčce vyčítá a podle toho se na sériový port odesílá informace buď `"plo locked\r"` a nebo `"plo isn't locked\r"`

## 4. Video demonstrující funkci

## 5. Závěr

## 6. Zdroje



