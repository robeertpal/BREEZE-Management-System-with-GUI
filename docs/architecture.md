# Architecture

## Entry point and build

`src/gui/main.cpp` creates `QApplication`, a splash screen, and `MainWindow`. It is the only `main()` in the supplied source. Console functions remain compiled into the application but have no separate executable entry point.

The original GUI includes implementation files directly: `main.cpp` includes `mainwindow.cpp`, which includes both login implementations; those include the agent and partner menus. The qmake project compiles this chain once through `main.cpp` and lists the included files as `DISTFILES`. Other GUI pages are independent translation units. This preserves the implementation while avoiding duplicate definitions from compiling included files separately.

## Source groups

| Directory | Responsibilities |
| --- | --- |
| `src/model` | `Member`, `Guest`, `Staff` and derived agents, ticket types, `Partner`, `PartnerStaff`, credentials, date/contact value objects, and `Bracelet<T>` |
| `src/core` | Ticket-code generation, `TicketFactory`, and templated fee strategies |
| `src/service` | JSON loaders/savers and guest, staff, credential, and partner console operations |
| `src/validation` | Email, phone, date, age, bracelet-code, and username checks |
| `src/exception` | `Exception` and domain-specific subclasses |
| `src/gui` | Widgets, login, role menus, and operational pages |

These directories describe existing responsibilities. GUI pages call persistence managers and modify model objects directly; service files also contain console I/O. There is no enforced service boundary or separate backend.

## Domain behavior

- `Ticket` and `Staff` provide abstract interfaces, specialized by concrete ticket and agent classes.
- `TicketFactory` constructs ticket types from their type codes.
- `PhoneNumber::clone()` supports copying through its virtual interface; `Email` does not implement `clone()`.
- `Email` and `PhoneNumber` assignments take their argument by value and swap their stored string.
- Staff administrator permissions depend on ID `0`; partner administrator permissions depend on partner staff ID `0`.
- Access restriction removes the guest record rather than persisting a separate blocked status.
- Fee strategies are present in the source. The partner-sale GUI independently uses a fixed 10% commission; it does not dispatch through those strategies.
- `PartnerStaff` is retained even though the GUI primarily represents partner workers through credential records.

## Persistence and assets

Managers serialize JSON files relative to the process working directory. Schemas, filenames, plaintext credential handling, and loading/saving logic are unchanged. Sales save guest and partner files separately; this is not a transactional store.

Original images reside in `assets/`. `assets.qrc` embeds them under `/assets`; GUI image references use that resource prefix. The source also attempts to load a Montserrat font resource, but no font was supplied. That original call is retained and the application uses available fonts if the resource cannot be loaded.

## Cleanup scope

All 66 original non-generated C++ source/header files are retained. Changes inside C++ files are limited to image resource paths. The qmake configuration now explicitly selects C++17, uses portable source/include paths, includes all original model sources and headers, and excludes separately compiling the GUI files included by other source files. Existing business logic and console operations have not been rewritten.

Generated files and IDE state are stored in ignored `.local-backup/`; original JSON records are in ignored `data/local/`. Neither belongs in the published repository.
