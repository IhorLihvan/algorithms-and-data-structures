#import "@preview/marginalia:0.3.1" as marginalia
#import "common.typ": make-index

#set page(
  paper: "a4",
  header: context {
    if here().page() == 1 { return }
    align(center, text(size: 9pt, fill: luma(120))[Технічна книга курсової роботи — Алгоритми і Структури Даних])
  },
  footer: context {
    if here().page() == 1 { return }
    align(center, text(size: 10pt)[#here().page()])
  }
)

// Поля сторінки та колонка приміток праворуч (стиль kaobook)
// Праве поле = far + width + sep = 6.5cm
#show: marginalia.setup.with(
  inner: (far: 2.5cm, width: 0cm, sep: 0cm),
  outer: (far: 1.3cm, width: 4.5cm, sep: 0.7cm),
  top: 2.5cm,
  bottom: 2.5cm,
)


// Налаштування шрифтів та тексту
#set text(font: "Liberation Serif", size: 11pt, lang: "uk")
#set par(justify: true, leading: 0.65em)
#set heading(numbering: "1.1.")

// Підпис таблиці --- над таблицею, підпис рисунка --- під рисунком
#show figure.where(kind: table): set figure.caption(position: top)
// Довга таблиця може переходити на наступну сторінку
#show figure.where(kind: table): set block(breakable: true)

// Посилання в тексті --- скороченням, яке не відмінюється:
// «у табл. 4», «на рис. 2», «з розд. 1.4.1», а не «у Таблиця 4».
// У підписах лишаються повні слова «Таблиця 4», «Рисунок 2».
// Явний підпис (@ch-table[розділу]) і #num(...) правило не змінює.
#show ref: it => {
  let el = it.element
  if el == none or it.supplement != auto {
    return it
  }
  if el.func() == heading {
    return ref(it.target, supplement: [розд.])
  }
  if el.func() == figure {
    if el.kind == table {
      return ref(it.target, supplement: [табл.])
    }
    if el.kind == image {
      return ref(it.target, supplement: [рис.])
    }
    if el.kind == raw {
      return ref(it.target, supplement: [лістинг])
    }
    if el.kind == "theorem" {
      return ref(it.target, supplement: [теорема])
    }
  }
  it
}

// --- ТИТУЛЬНА СТОРІНКА ---
#page(margin: 2.5cm)[
  #v(4cm)
  #align(center)[
    #text(size: 20pt, weight: "bold")[КУРСОВА РОБОТА] \
    #v(1cm)
    #text(size: 14pt)[Тема: Дослідження та реалізація алгоритмів обробки даних] \
    #v(2cm)
    #text(size: 12pt)[Виконав: студент групи КН-...\
    Ігор Ліхван] \
    #v(1cm)
    #text(size: 12pt)[Керівник: Літинський Святослав] \
    #v(3cm)
    #text(size: 10pt)[Львів --- 2026]
  ]
]

#pagebreak()

// --- ЗМІСТ ---
#outline(title: "Зміст", indent: 1.5em, depth: 3)
#pagebreak()

// --- ПІДКЛЮЧЕННЯ РОЗДІЛІВ ---
#include "table_chapter.typ"
#include "hash_table_chapter.typ"

// --- СПИСОК ЛІТЕРАТУРИ (спільний для всіх розділів) ---
#pagebreak()
#bibliography(
  "references.bib",
  title: "Список літератури",
  style: "gost-r-705-2008-numeric",
)

// --- АЛФАВІТНИЙ ПОКАЖЧИК (терміни позначено через #idx у розділах) ---
#pagebreak()
#heading(numbering: none)[Алфавітний покажчик]
#make-index()