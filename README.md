# BREEZE Management System

C++/Qt festival management application with role-based workflows, JSON persistence and object-oriented architecture.

## Overview

BREEZE manages festival guests, internal staff, partners, ticketing and check-in, wristband top-ups, sales, and credentials. Multiple operational roles share domain objects and persist records in JSON files.

The **Qt GUI is the current application**, with Romanian interface text. Console-oriented operations remain in the same codebase; there is no separate CLI entry point or standalone CLI build target.

## Tech Stack

- C++17 (explicitly configured in qmake)
- Qt 6 Widgets; the original build configuration used Qt 6.9.0
- nlohmann/json, a header-only JSON library
- JSON file persistence
- Object-oriented programming
- Qt GUI and retained console operations

## Key Features

- Guest registration, ticket code generation, and ticket check-in
- Wristband top-ups, balance inspection, and payments
- Access restriction by removing a guest record
- Staff, credential, and partner management
- Sales processing and guest/partner balance updates
- Persistent JSON storage, input validation, and domain-specific exceptions

Some management operations are implemented in the retained console functions but are not exposed by the GUI menus.

## Roles and Workflows

### BREEZE Staff

| Role | GUI capabilities |
| --- | --- |
| Ticket Agent | List/register guests, check tickets in, inspect tickets, and process wristband payments |
| Security Agent | Inspect tickets and restrict access |
| Top-Up Agent | Add funds, inspect balances, and process wristband payments |
| BREEZE Admin | All staff GUI actions, including adding and deleting staff; identified by staff ID `0` |

The console operations also include staff detail updates and credential management.

### Partners

| Role | GUI capabilities |
| --- | --- |
| Partner Worker | Record sales and inspect the partner balance |
| Partner Admin | Worker actions plus adding/deleting partner employees; identified by staff ID `0` within a partner |

Partner creation and additional credential changes are retained in the console operations.

## Object-Oriented Design

`Ticket` and `Staff` are abstract base classes with virtual functions and derived ticket/agent types. The application uses runtime polymorphism, `std::shared_ptr`, and `dynamic_cast` for type-dependent behavior. `PhoneNumber` implements a virtual `clone()`; `Email` and `PhoneNumber` use copy-and-swap assignment.

Other source-level examples include `TicketFactory`, the templated `Bracelet`, fee strategies, dedicated validators, and custom exceptions. See [architecture](docs/architecture.md) for their actual scope.

## Validation and Error Handling

Dedicated functions validate email addresses, phone numbers, dates, adult eligibility, bracelet/ticket codes, and username uniqueness. `Credentials` enforces a minimum password length of six characters.

Custom exceptions derive from `Exception`, which derives from `std::exception`:

- `InvalidEmail`
- `InvalidPhoneNumber`
- `InvalidDate`
- `AgeRestriction`
- `InvalidTicket`
- `WeakPassword`
- `InsufficientFunds`
- `UsernameAlreadyExists`

Validation and error handling differ between operations; these classes do not imply every UI path applies every check.

## Persistence

Managers read and write JSON arrays using nlohmann/json. The application uses `guests.json`, `staff.json`, `credentials.json`, `partners.json`, and `partners_credentials.json`, resolved relative to the **process working directory**. There is no database.

The repository's `data/guests.json` and `data/partners.json` are empty starter arrays. Runtime records belong in ignored `data/local/`. Original local records have been preserved there during cleanup, including credential files; they are not publication material. Password storage remains plaintext, as implemented. See [data setup and schemas](data/README.md).

## Example Workflow

1. A Ticket Agent registers a guest.
2. Personal details pass the applicable input validation.
3. The application generates a ticket code.
4. A Top-Up Agent adds funds to the wristband.
5. A Partner Worker records and confirms a sale.
6. The GUI deducts the sale amount plus its 10% commission from the guest, credits the sale amount to the partner, and saves both JSON files.

## Architecture

The existing code separates domain models, operation functions, JSON managers, validators, exceptions, and Qt screens. GUI pages call managers and domain objects directly; console operation functions also contain user interaction. These are organizational groups, not strictly isolated architectural layers.

## Repository Structure

```text
breeze-management-system/
├── README.md
├── .gitignore
├── breeze.pro
├── src/
│   ├── core/          # Ticket generator, ticket factory, fee strategies
│   ├── model/         # Guests, staff, partners, tickets, and value objects
│   ├── service/       # JSON managers and console operation functions
│   ├── validation/    # Dedicated input validators
│   ├── exception/     # Domain exceptions
│   └── gui/           # Qt entry point, menus, and pages
├── data/
│   ├── guests.json
│   ├── partners.json
│   └── README.md
├── docs/
│   ├── architecture.md
│   └── references.md
└── assets/
    ├── assets.qrc
    └── *.jpg          # Original image assets
```

Local runtime data, build output, and the ignored `.local-backup/` directory are omitted from this publication tree. No screenshots were supplied, so no screenshot directory is included.

## Build and Run

### Requirements

- A C++17 compiler supported by the installed Qt 6 kit (for example, Clang on macOS)
- Qt 6 with Widgets, qmake, and resource compiler; original project version: 6.9.0
- nlohmann/json headers, available on the compiler include path or supplied to qmake
- `make`

The previous README mentioned g++ 7+, but this is not a verified minimum for the Qt 6 GUI. Use a compiler compatible with your Qt installation.

### macOS

Install Qt 6 and put its `bin` directory on `PATH`, so `qmake -v` reports Qt 6. Install the JSON headers with Homebrew:

```bash
brew install nlohmann-json
mkdir -p build
cd build
qmake ../breeze.pro "NLOHMANN_JSON_INCLUDE=$(brew --prefix nlohmann-json)/include"
make
cd ..
```

If the JSON headers are already on the compiler's include path, use `qmake ../breeze.pro` without the extra argument. On other platforms, use the same qmake project with a suitable Qt 6 toolchain and JSON include directory.

Prepare the files described in [data/README.md](data/README.md), then run from that data directory:

```bash
cd data/local
../../build/breeze.app/Contents/MacOS/breeze
```

For a Unix build without a macOS app bundle, the executable is normally `../../build/breeze`. In Qt Creator, set the working directory to `data/local` as well. Images are embedded using Qt resources and do not depend on the launch directory.

A fresh checkout contains no login credentials or preconfigured accounts. Provision local staff/partner records before signing in; there is no first-run account setup screen.

### Verification Status

The cleanup was checked for source preservation, local include/resource references, JSON validity, and exclusion of private/generated files. C++17 syntax checks passed for 17 non-Qt translation units that do not require the unavailable JSON header. A full GUI build and interactive run require Qt 6 and nlohmann/json, which were not available in the cleanup environment. No automated test suite is included.
