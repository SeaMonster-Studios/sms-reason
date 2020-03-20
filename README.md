# SeaMonster Studios ReasonML Mono Repo

## Publishing updates to packages

1. Sign into npm in your terminal, `npm login`
2. Run `lerna publish` and make version bump selection.

## Adding a new package to this mono repo

1. Duplicate one of the existing component packages
2. Update the "name" field in package.json and bsconfig.json (need to match)
3. Add your component, lib, etc.
4. Follow the steps in the [publishing updates](#user-content-publishing-updates-to-packages) section

### installing an npm package

- Within on of the ./packages, run `yarn add <pkgName>` like normal, then run `lerna bootstrap`
