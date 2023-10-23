### procrule (WIP) в разработке

Утилита для получения базовой информации о процессах в системе и
выполнения пользовательских скриптов на lua на основе этой информации.
Может пригодится в специфичных ситуациях собственных OOM или прибивать зависшие процессы


### TODO

 * Сбор базовой информации
 * Предрасчитанные значения потребления
    * памяти
    * процессора
 * Три вариации сборки
   * утилита коммандной строки `$procrule userscript.lua`
   * библиотека для **lua** `local procrule = require('procrule')`
   * библиотека для **С**   `procrule(void(*usercallback)(struct procrule))`

-------------

Идея утилиты навеняна темой форума
 * https://www.linux.org.ru/forum/admin/17380314

Суть в том что пользователь хочет пришибать по затылку процессы которые жрут 100%
процессора, особенность в том что у него тысячи процессов и некоторые сотни из них
в определённых условиях создают такую нагрузку что вся система встаёт раком по его
словам и нужно оперативно и **быстро** вычислять такие процессы закрывая их или снижать приоритет. Это
странный случай, но прикольный. Наверное подобная примитивная парсилка `/proc/pid/stat`
будет нужна только для подобных редких случаев, тоесть не нужна по сути :D Но пусть
будет, как минимум сгодится как заготовочка и наглядный пример как парсить `/proc/`
