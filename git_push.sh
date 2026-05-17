#!/bin/sh

echo Pushing files to repository...

git add --all
git commit -m "New features"
git push -u origin main

echo DONE
