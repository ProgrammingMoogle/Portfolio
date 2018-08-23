When adding new sounds (music or effects) to manifest files,
you need to specify the filename and loop status.
The hash sign (#) is used for line comment

------------- AUDIO MUST BE IN MP3 FORMAT -------------

filename: name of the mp3 (do not include .mp3 in manifest files)
loop: the loop status of the sound (loop or noloop)

----------------------- EXAMPLES -----------------------

Add non-looping music "song.mp3" in manifest will look like:  song        noloop
Add looping music "loop_song.mp3" will look like:             loop_song     loop
Add non-looping effect "effect.mp3" will look like:           effect      noloop
Add looping sound effect "loop_effect.mp3" will look like:    loop_effect   loop