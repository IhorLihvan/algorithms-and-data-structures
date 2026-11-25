// Спільні допоміжні функції для всіх розділів книги.
// Підключення на початку розділу:  #import "common.typ": *
// Домовленості для авторів --- у файлі CONVENTIONS.md.

#import "@preview/marginalia:0.3.1" as marginalia

// --- Примітки на полях ---

// Примітка з номером: мітка¹ у тексті, «1» біля примітки
#let sidenote = marginalia.note.with(
  numbering: (..i) => super(numbering("1", ..i)),
  text-style: (size: 9pt, fill: luma(60)),
)
// Примітка без номера
#let marginnote = marginalia.note.with(
  counter: none,
  text-style: (size: 9pt, fill: luma(60)),
)

// --- Загальнотеоретичні операції ---

// Однаковий запис операцій у всій книзі: #op("INSERT", $x$) дає INSERT(x).
// Без аргументів: #op("CLEAR") дає CLEAR().
#let op(name, ..args) = [#raw(name)$(#args.pos().join($, $))$]

// --- Посилання на розділи колег ---

// Посилання на мітку, якої може ще не бути в книзі (розділ колеги ще
// не злито). Коли мітка з'явиться, підставиться номер розділу;
// до того виводиться fallback.
// Приклад: #chref(<ch-quicksort>)[розділ про сортування]
#let chref(target, fallback) = context {
  if query(target).len() > 0 {
    ref(target)
  } else {
    fallback
  }
}

// --- Код із файлів ---

// Лістинг із файлу в папці code/. Код у книзі завжди той самий, що
// компілюється і проходить тести. part --- фрагмент між рядком
// "// == part ==" і наступним таким рядком; без part --- увесь файл.
// Рядки-позначки "// == ... ==" у книзі не виводяться.
#let code-file(path, part: none, lang: "cpp") = {
  let is-marker(line) = {
    let t = line.trim()
    t.starts-with("// == ") and t.ends-with(" ==")
  }
  let lines = read(path).replace("\r\n", "\n").split("\n")
  let inside = part == none
  let found = part == none
  let out = ()
  for line in lines {
    if is-marker(line) {
      if part != none {
        inside = line.trim() == "// == " + part + " =="
        found = found or inside
      }
      continue
    }
    if inside {
      out.push(line)
    }
  }
  assert(found, message: "немає фрагмента " + repr(part) + " у " + path)
  while out.len() > 0 and out.first().trim() == "" {
    out.remove(0)
  }
  while out.len() > 0 and out.last().trim() == "" {
    out.pop()
  }
  block(breakable: true, width: 100%, above: 0.9em, below: 0.9em, {
    text(size: 7.5pt, fill: luma(110), raw(path.trim("code/", at: start)))
    v(-0.6em)
    raw(out.join("\n"), lang: lang, block: true)
  })
}

// Результат роботи програми з текстового файлу
#let program-output(path) = block(
  width: 100%,
  fill: luma(245),
  inset: 8pt,
  radius: 3pt,
  align(left, raw(read(path).replace("\r\n", "\n").trim(), block: true)),
)

// --- Теореми ---

// Теорема з наскрізним номером. Посилання: @thm-... дає «Теорема 3».
// Приклад:
//   #theorem(source: [@clrs[теорема 11.1]])[Текст теореми.] <thm-chain>
#let theorem(source: none, body) = figure(
  kind: "theorem",
  supplement: [Теорема],
  numbering: "1",
  outlined: false,
  block(width: 100%, align(left)[
    *Теорема #context counter(figure.where(kind: "theorem")).display()*
    #if source != none [#source]. #body
  ]),
)

// Лише номер об'єкта з посиланням, без слова: «з теорем #num(<a>) і #num(<b>)»
#let num(target) = ref(target, supplement: none)

// Доведення або ідея доведення; закінчується знаком □
#let proof(title: [Доведення], body) = block(width: 100%)[
  _#title._ #body #h(1fr) $square$
]

// --- Алфавітний покажчик ---

// Позначка терміна: #idx("хеш-функція") або з підтерміном:
// #idx("хеш-функція", "метод ділення"). У тексті нічого не виводить.
#let idx(..terms) = [#metadata(terms.pos())<index-entry>]

// Український алфавіт для сортування (Unicode ставить ґ, є, і, ї не на свої місця)
#let _alphabet = "абвгґдеєжзиіїйклмнопрстуфхцчшщьюя".clusters()
#let _latin = "abcdefghijklmnopqrstuvwxyz".clusters()

#let _sort-key(term) = lower(term).clusters().map(c => {
  let i = _alphabet.position(x => x == c)
  if i != none { return str(100 + i) }
  let j = _latin.position(x => x == c)
  if j != none { return str(200 + j) }
  if c.match(regex("[0-9]")) != none { return "0" + c + "0" }
  "000"   // пробіл, дефіс, апостроф тощо
}).join()

// Номери сторінок (без повторів) з посиланнями на перше входження
#let _pages(locs) = {
  let seen = ()
  let links = ()
  for loc in locs.sorted(key: l => l.page()) {
    let p = loc.page()
    if p not in seen {
      seen.push(p)
      links.push(link(loc, str(p)))
    }
  }
  links.join(", ")
}

#let make-index(n-columns: 2) = context {
  // терміни -> (locs: позначки самого терміна, subs: підтерміни)
  let terms = (:)
  for e in query(<index-entry>) {
    let main = e.value.at(0)
    let entry = terms.at(main, default: (locs: (), subs: (:)))
    if e.value.len() == 1 {
      entry.locs.push(e.location())
    } else {
      let sub = e.value.at(1)
      let sub-locs = entry.subs.at(sub, default: ())
      sub-locs.push(e.location())
      entry.subs.insert(sub, sub-locs)
    }
    terms.insert(main, entry)
  }

  let sorted = terms.keys().sorted(key: _sort-key)
  set par(justify: false, first-line-indent: 0pt, hanging-indent: 1.5em, spacing: 0.45em)
  set text(size: 10pt)
  columns(n-columns, gutter: 1.5em, {
    let letter = none
    for main in sorted {
      let first = upper(main.clusters().first())
      if first != letter {
        letter = first
        v(0.8em, weak: true)
        block(sticky: true, text(weight: "bold", size: 11pt, first))
      }
      let entry = terms.at(main)
      par[#main#if entry.locs.len() > 0 [, #_pages(entry.locs)]]
      for sub in entry.subs.keys().sorted(key: _sort-key) {
        par[#h(1em)#sub, #_pages(entry.subs.at(sub))]
      }
    }
  })
}
