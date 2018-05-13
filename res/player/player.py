from music21 import *
import sys
import json
import platform


def deserializeForPlaying(js):
    
  metrics = js['metrics']
  comp_part_json = js['parts']
  
  sc = stream.Score();
  
  for partjson in comp_part_json:
    new_part = stream.Part()
    sc.insert(0, new_part)
    for new_note_json in partjson["events"]:
      if 'pitch' in new_note_json:
        newpitch = int(new_note_json['pitch'])
        if newpitch is 127:
          n = note.Rest()
        else:
          n = note.Note()
          n.pitch.midi = newpitch
      
      if 'pitches' in new_note_json:
        pitches = new_note_json['pitches']
        n = chord.Chord(pitches)
      dur = new_note_json['duration'] / 96.0;
      n.duration.quarterLength = dur

      new_part.append(n)
  
  for met in metrics:
    ts = meter.TimeSignature()
    ts.numerator = met['timeSignature']['num']
    ts.denominator = met['timeSignature']['denom']
    a = tempo.MetronomeMark('tempoChange', met['tempo'])
    intervals = met['key']['intervals']
    tonic = met['key']['tonic']
    scale = [pitch.Pitch()]
    scale[0].midi = tonic
    for i in intervals:
      p = pitch.Pitch()
      p.midi = scale[-1].midi + i
      scale.append(p)
    ks = key.KeySignature() #SAM HALP
  
  return sc

if(platform.system() == 'Linux'):
  environment.set('midiPath', '/usr/bin/timidity')

deserializeForPlaying(json.load(sys.stdin)).show('midi')

