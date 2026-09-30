# Runtime data

The application reads and writes five JSON files in its current working directory. Persistence code and schemas are unchanged. Run with `data/local/` as the working directory to keep runtime records out of version control.

The checked-in `guests.json` and `partners.json` contain empty arrays only. During this cleanup, all five original JSON files were moved unchanged into ignored `local/`, preserving existing local use without publishing credentials or personal records.

## Fresh checkout

From the repository root:

```bash
mkdir -p data/local
cp data/guests.json data/local/guests.json
cp data/partners.json data/local/partners.json
printf '[]\n' > data/local/staff.json
printf '[]\n' > data/local/credentials.json
printf '[]\n' > data/local/partners_credentials.json
```

Use those initialization commands only for a fresh checkout: they overwrite local files. Empty arrays initialize the files, but provide no accounts to log into. The GUI has no initial account provisioning screen. Populate local JSON records using the schemas below before signing in. Keep passwords and personal details solely in `local/`; do not force-add ignored files.

## Existing schemas

Each file contains a JSON array of objects:

| File | Fields |
| --- | --- |
| `guests.json` | `firstName`, `lastName`, `email`, `phone`, `birthDate`, `nationality`, `ticketType`, `ticketCode`, `braceletBalance`, `checkedIn` |
| `staff.json` | `id`, `firstName`, `lastName`, `email`, `phone`, `birthDate`, `nationality`, `role` |
| `credentials.json` | `id`, `username`, `password` |
| `partners.json` | `partnerID`, `partnerName`, `partnerType`, `partnerBalance` |
| `partners_credentials.json` | `partnerID`, `staffID`, `username`, `password` |

Names, contact fields, dates, roles, ticket fields, and credentials are strings. IDs are integers, balances are numbers, and `checkedIn` is boolean. Dates use `DD/MM/YYYY`. Ticket types are `GeneralAccess`, `GeneralVIP`, or `VIPMeetAndGreet`. Staff roles are `TicketAgent`, `SecurityAgent`, or `TopUpAgent`.

A BREEZE administrator is a staff record with `id: 0`, a valid staff role, and a matching credential record with `id: 0`. Other staff credentials must likewise match staff IDs. Member data is validated while loading, including adult eligibility. Staff passwords must contain at least six characters.

Partner credentials reference an existing `partnerID`; `staffID: 0` grants that partner's administrator menu. Provision a partner record and matching local credentials to access partner workflows. No credential values are distributed with this repository.

Passwords remain plaintext in the existing implementation. The files are local application state, not a database, and no migration or authentication redesign is part of this cleanup.
