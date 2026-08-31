/**
 * intgmdbot - High-Performance C++20 Telegram GFM Bot Landing Script
 * Supports i18n (RU, BE, EN), automatic browser lang detection,
 * Theme switching (Dark/Light), and Interactive GFM Demo controller.
 */

const i18n = {
    ru: {
        docTitle: "mdintgbot — Telegram Markdown",
        navFeatures: "Возможности",
        navArchitecture: "Архитектура",
        navAuthor: "Об авторе",
        heroBadge: "C++20 бот для конвертации md в сообщения",
        heroTitle: "mdintgbot — Telegram Markdown Bot",
        heroSubtitle: "Телеграм бот для конвертации текста в формате markdown в красивые сообщения телеграмм. Написан на C++20, использует API 10.1 RichMessages. Доступен в Telegram как @mdintgbot.",
        btnTelegram: "Запустить @mdintgbot",
        btnGitHub: "Исходный код на GitHub",
        btnDocs: "Документация C++",
        
        // Demo Tabs
        tabRich: "Rich Message (API 10.1)",
        tabBalance: "Автобалансировка тегов",
        tabAccumulator: "Аккумулятор ЛС",
        tabFallback: "3-уровневый Фолбек",

        demoRichDesc: "Формирование нативного JSON-пейлоада <code>{\"markdown\": sanitized}</code> с полной поддержкой таблиц, списков и кодовых блоков GFM.",
        demoBalanceDesc: "Функция <code>balance_markdown()</code> автоматически закрывает незавершенные теги <code>```</code>, <code>`</code>, <code>**</code> и <code>~~</code>, исключая ошибки <code>RICH_MESSAGE_MARKDOWN_INVALID</code>.",
        demoAccumulatorDesc: "Модуль <code>MessageAccumulator</code> собирает несколько быстро отправленных сообщений пользователя в одно в пределах тайм-аута, экономя лимиты Telegram.",
        demoFallbackDesc: "Если Rich Message отклоняется сервером Telegram, движок последовательно выполняет фолбек на парсер <code>MarkdownV2 AST</code>, а затем на чистый текст.",

        demoStatus: "Статус: активен 🟢",
        demoSpeed: "Задержка: < 1 мс",

        // Features Section
        featuresTag: "Возможности",
        featuresTitle: "Почему mdintgbot?",
        fTitle1: "Bot API 10.1 Rich Messages",
        fDesc1: "Прямая отправка нативного GFM-размеченного текста без ограничений устаревшего формата MarkdownV2.",
        fTitle2: "Авто-балансировка тегов",
        fDesc2: "Интеллектуальный алгоритм закрытия парных синтаксических конструкций исключает падения из-за битого Markdown.",
        fTitle3: "Санитайзер NBSP",
        fDesc3: "Автоматическая замена неразрывных пробелов (0xC2 0xA0) на обычные для корректного отображения списков GFM.",
        fTitle4: "Аккумулятор сообщений",
        fDesc4: "Буферизация потока коротких сообщений от пользователя и их объединение в один красивый форматированный пост.",
        fTitle5: "3-уровневая надежность",
        fDesc5: "Каскадная отправка: Telegram Rich Message → MarkdownV2 AST Parser → Plain Text. Сообщение гарантированно дойдет.",
        fTitle6: "C++20 Runtime",
        fDesc6: "Сборка на CMake 3.20+, OpenSSL, cpp-httplib и nlohmann_json. Минимальное потребление RAM и мгновенный отклик.",

        // Architecture Section
        archTag: "Архитектура",
        archTitle: "Устройство движка C++20",
        archP1Title: "Message Accumulator Pipeline",
        archP1Desc: "Асинхронная очередь сообщений с фоновым потоком таймера. Объединяет последовательные сообщения от одного пользователя в единый буфер перед отправкой.",
        archP2Title: "AST-Парсер MarkdownV2",
        archP2Desc: "Собственный AST парсер для транспиляции GFM в безопасные escape-последовательности MarkdownV2 на случай недоступности Rich Message API.",
        archP3Title: "Ротируемое логирование",
        archP3Desc: "Модуль logger.cpp ведает записью события с уровнями INFO, WARNING, ERROR и CRITICAL в файл bot.log.",
        archP4Title: "Конфигурируемый JSON",
        archP4Desc: "Гибкая настройка через config.json: таймауты аккумуляции, токены и режимы парсинга без пересборки бинарного файла.",

        // Author Section
        authorTag: "Создатель",
        authorTitle: "Об авторе",
        authorName: "Nikitos",
        authorBio: "Разработчик проекта mdintgbot. Любитель IT и DIY.",
        authorBtn: "Об авторе ↗"
    },

    be: {
        docTitle: "mdintgbot — Telegram Markdown",
        navFeatures: "Магчымасці",
        navArchitecture: "Архітэктура",
        navAuthor: "Пра аўтара",
        heroBadge: "C++20 бот для канвертацыі md у паведамленні",
        heroTitle: "mdintgbot — Telegram Markdown Bot",
        heroSubtitle: "Тэлеграм бот для канвертацыі тэксту ў фармаце markdown у прыгожыя паведамленні тэлеграм. Напісаны на C++20, выкарыстоўвае API 10.1 RichMessages. Даступны ў Telegram як @mdintgbot.",
        btnTelegram: "Запусціць @mdintgbot",
        btnGitHub: "Выходны код на GitHub",
        btnDocs: "Дакументацыя C++",
        
        tabRich: "Rich Message (API 10.1)",
        tabBalance: "Аўтабалансіроўка тэгаў",
        tabAccumulator: "Акумулятар ЛП",
        tabFallback: "3-узроўневы Фолбек",

        demoRichDesc: "Фармаванне натыўнага JSON-пейлоада <code>{\"markdown\": sanitized}</code> з поўнай падтрымкай табліц, спісаў і кодавых блокаў GFM.",
        demoBalanceDesc: "Функцыя <code>balance_markdown()</code> аўтаматычна зачыняе незавершаныя тэгі <code>```</code>, <code>`</code>, <code>**</code> і <code>~~</code>.",
        demoAccumulatorDesc: "Модуль <code>MessageAccumulator</code> збірае некалькі хутка адпраўленых паведамленняў у адно ў межах таймаўту.",
        demoFallbackDesc: "Калі Rich Message адхіляецца сервером Telegram, рухавік паслядоўна выконвае фолбек на парсер <code>MarkdownV2 AST</code>.",

        demoStatus: "Статус: актыўны 🟢",
        demoSpeed: "Затрымка: < 1 мс",

        featuresTag: "Магчымасці",
        featuresTitle: "Чаму mdintgbot?",
        fTitle1: "Bot API 10.1 Rich Messages",
        fDesc1: "Прамая адпраўка натыўнага GFM-размечанага тэксту без абмежаванняў састарэлага фармату MarkdownV2.",
        fTitle2: "Аўта-балансіроўка тэгаў",
        fDesc2: "Інтэлектуальны алгарытм закрыцця парных сінтаксічных канструкцый выключае памылкі.",
        fTitle3: "Санітайзер NBSP",
        fDesc3: "Аўтаматычная замена неразрыўных прабелаў (0xC2 0xA0) на звычайныя для карэктнага адлюстравання спісаў GFM.",
        fTitle4: "Акумулятар паведамленняў",
        fDesc4: "Буферызацыя патоку кароткіх паведамленняў ад карыстальніка і іх аб'яднанне ў адзін прыгожы пост.",
        fTitle5: "3-узроўневая надзейнасць",
        fDesc5: "Каскадная адпраўка: Telegram Rich Message → MarkdownV2 AST Parser → Plain Text.",
        fTitle6: "C++20 Runtime",
        fDesc6: "Зборка на CMake 3.20+, OpenSSL, cpp-httplib і nlohmann_json. Мінімальнае спажыванне RAM і імгненны водгук.",

        archTag: "Архітэктура",
        archTitle: "Уладкаванне рухавіка C++20",
        archP1Title: "Message Accumulator Pipeline",
        archP1Desc: "Асінхронная чарга паведамленняў з файлавым патокам таймера. Аб'ядноўвае паслядоўныя паведамленні ў адзін буфер.",
        archP2Title: "AST-Парсер MarkdownV2",
        archP2Desc: "Уласны AST парсер для транспіляцыі GFM у бяспечныя escape-паслядоўнасці MarkdownV2.",
        archP3Title: "Ратуемае лагіраванне",
        archP3Desc: "Модуль logger.cpp вядзе запіс падзей з узроўнямі INFO, WARNING, ERROR і CRITICAL у файл bot.log.",
        archP4Title: "Канфігуруемы JSON",
        archP4Desc: "Гнуткая настройка праз config.json: таймауты акумуляцыі, токены і рэжымы парсінгу.",

        authorTag: "Стваральнік",
        authorTitle: "Пра аўтара",
        authorName: "Nikitos",
        authorBio: "Распрацоўшчык праекта mdintgbot. Аматар IT і DIY.",
        authorBtn: "Пра аўтара ↗"
    },

    en: {
        docTitle: "mdintgbot — Telegram Markdown",
        navFeatures: "Features",
        navArchitecture: "Architecture",
        navAuthor: "Author",
        heroBadge: "C++20 Bot to convert md into messages",
        heroTitle: "mdintgbot — Telegram Markdown Bot",
        heroSubtitle: "Telegram bot for converting markdown text into beautiful Telegram messages. Written in C++20, powered by API 10.1 RichMessages. Available on Telegram as @mdintgbot.",
        btnTelegram: "Launch @mdintgbot",
        btnGitHub: "GitHub Source Code",
        btnDocs: "C++ Documentation",
        
        tabRich: "Rich Message (API 10.1)",
        tabBalance: "Tag Balancer",
        tabAccumulator: "DM Accumulator",
        tabFallback: "3-Tier Fallback",

        demoRichDesc: "Builds native JSON payload <code>{\"markdown\": sanitized}</code> with full GFM tables, lists, and code block support.",
        demoBalanceDesc: "The <code>balance_markdown()</code> algorithm automatically closes unclosed <code>```</code>, <code>`</code>, <code>**</code>, and <code>~~</code> tags to prevent <code>RICH_MESSAGE_MARKDOWN_INVALID</code> errors.",
        demoAccumulatorDesc: "The <code>MessageAccumulator</code> module batches rapid consecutive DMs from a user into a single formatted post within a configurable timeout.",
        demoFallbackDesc: "If a Rich Message is rejected by Telegram API, the engine seamlessly falls back to <code>MarkdownV2 AST Parser</code> and then plain text.",

        demoStatus: "Status: Active 🟢",
        demoSpeed: "Latency: < 1 ms",

        featuresTag: "Features",
        featuresTitle: "Why mdintgbot?",
        fTitle1: "Bot API 10.1 Rich Messages",
        fDesc1: "Native GFM markup delivery without the syntax restrictions of legacy MarkdownV2.",
        fTitle2: "Auto Tag Balancing",
        fDesc2: "Smart delimiter tracking prevents bot crashes caused by malformed user Markdown syntax.",
        fTitle3: "NBSP Sanitizer",
        fDesc3: "Replaces non-breaking spaces (0xC2 0xA0) with standard spaces to ensure proper GFM list indentation.",
        fTitle4: "Message Accumulator",
        fDesc4: "Buffers rapid single-line DMs and merges them into one clean, structured rich message.",
        fTitle5: "3-Tier Reliability",
        fDesc5: "Cascading pipeline: Telegram Rich Message → MarkdownV2 AST Parser → Plain Text.",
        fTitle6: "C++20 Runtime",
        fDesc6: "Built with CMake 3.20+, OpenSSL, cpp-httplib, and nlohmann_json. Extremely low memory footprint and sub-millisecond response time.",

        archTag: "Architecture",
        archTitle: "C++20 Engine Internals",
        archP1Title: "Message Accumulator Pipeline",
        archP1Desc: "Thread-safe asynchronous message queue with background timer thread. Combines consecutive user messages into a unified buffer.",
        archP2Title: "MarkdownV2 AST Parser",
        archP2Desc: "Custom AST parser that transpiles GFM into safely escaped MarkdownV2 entities as a secondary fallback.",
        archP3Title: "Rotating File Logger",
        archP3Desc: "High-speed logger (logger.cpp) supporting INFO, WARNING, ERROR, and CRITICAL log levels outputting to bot.log.",
        archP4Title: "JSON Configuration",
        archP4Desc: "Dynamic configuration via config.json: adjust accumulation timeouts, bot tokens, and parse modes without recompiling.",

        authorTag: "Creator",
        authorTitle: "About Author",
        authorName: "Nikitos",
        authorBio: "Developer of the intgmdbot project. IT & DIY enthusiast.",
        authorBtn: "About Author ↗"
    }
};

function detectLanguage() {
    const saved = localStorage.getItem('intgmdbot_lang');
    if (saved && i18n[saved]) return saved;

    const browserLangs = navigator.languages || [navigator.language || navigator.userLanguage];
    const isBelarusian = browserLangs.some(l => (l || '').toLowerCase().includes('be') || (l || '').toLowerCase().includes('by')) 
                      || (Intl.DateTimeFormat().resolvedOptions().timeZone || '').includes('Minsk');

    return isBelarusian ? 'be' : 'ru';
}

let currentLang = detectLanguage();

document.addEventListener('DOMContentLoaded', () => {
    // 1. Language detection using SwipeBeat timezone & locale scheme
    setLanguage(detectLanguage());

    // 2. Theme persistence
    const savedTheme = localStorage.getItem('intgmdbot_theme') || 'dark';
    setTheme(savedTheme);

    // 3. Dropdown outside click handler
    document.addEventListener('click', (e) => {
        if (!e.target.closest('.lang-dropdown')) {
            const menu = document.getElementById('langMenu');
            if (menu) menu.classList.remove('show');
        }
    });
});

function toggleLangMenu(event) {
    event.stopPropagation();
    const menu = document.getElementById('langMenu');
    if (menu) menu.classList.toggle('show');
}

function setLanguage(lang) {
    if (!i18n[lang]) return;
    currentLang = lang;
    localStorage.setItem('intgmdbot_lang', lang);
    document.body.setAttribute('data-lang', lang);

    // Update Dropdown Label
    const labels = { ru: '🇷🇺 RU', be: '🇧🇾 BE', en: '🇬🇧 EN' };
    const labelSpan = document.getElementById('currentLangLabel');
    if (labelSpan) labelSpan.textContent = labels[lang];

    // Apply translations
    const elements = document.querySelectorAll('[data-i18n]');
    elements.forEach(el => {
        const key = el.getAttribute('data-i18n');
        if (i18n[lang][key]) {
            el.innerHTML = i18n[lang][key];
        }
    });

    const menu = document.getElementById('langMenu');
    if (menu) menu.classList.remove('show');
}

function toggleTheme() {
    const currentTheme = document.body.getAttribute('data-theme') || 'dark';
    const newTheme = currentTheme === 'dark' ? 'light' : 'dark';
    setTheme(newTheme);
}

function setTheme(theme) {
    document.body.setAttribute('data-theme', theme);
    localStorage.setItem('intgmdbot_theme', theme);

    const themeIcon = document.getElementById('themeIcon');
    if (themeIcon) {
        themeIcon.className = theme === 'dark' ? 'fas fa-moon' : 'fas fa-sun';
    }
}

// Interactive Demo Tab Switcher
function switchDemoTab(tabName) {
    const tabBtns = document.querySelectorAll('.stage-tab-btn');
    const tabPanels = document.querySelectorAll('.demo-tab-panel');

    tabBtns.forEach(btn => btn.classList.remove('active'));
    tabPanels.forEach(panel => panel.classList.remove('active'));

    const activeBtn = document.querySelector(`.stage-tab-btn[onclick*="${tabName}"]`);
    const activePanel = document.getElementById(`panel-${tabName}`);

    if (activeBtn) activeBtn.classList.add('active');
    if (activePanel) activePanel.classList.add('active');
}
