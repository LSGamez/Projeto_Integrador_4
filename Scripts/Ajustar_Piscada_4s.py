"""Uma piscada a cada quatro segundos na sequencia usada por Cena_Menu."""
import unreal as u,pathlib,datetime,shutil,json,traceback
ROOT=pathlib.Path(u.Paths.project_dir()).resolve()
report={'complete':False}
def run():
    level=u.get_editor_subsystem(u.LevelEditorSubsystem)
    if not level.load_level('/Game/Cenas/Cena_Menu'):raise RuntimeError('Falha ao abrir Cena_Menu.')
    actors=u.get_editor_subsystem(u.EditorActorSubsystem).get_all_level_actors()
    matches=[a for a in actors if isinstance(a,u.LevelSequenceActor) and a.get_actor_label()=='Luz_Piscando']
    if len(matches)!=1:raise RuntimeError('Luz_Piscando ausente ou ambiguo; nada alterado.')
    actor=matches[0];seq=actor.get_sequence()
    if not seq:raise RuntimeError('O ator esta sem sequencia.')
    intensity=[];brightness=[]
    for binding in seq.get_bindings():
        for track in binding.get_tracks():
            if isinstance(track,u.MovieSceneFloatTrack) and str(track.get_property_name()).lower()=='intensity':
                for section in track.get_sections():
                    channels=section.get_all_channels()
                    if channels:intensity.append((section,channels[0]))
    for track in seq.get_tracks():
        if isinstance(track,u.MovieSceneMaterialParameterCollectionTrack):
            mpc=track.get_editor_property('mpc')
            if mpc and mpc.get_name()=='MPC_Luz':brightness.extend(track.get_sections())
    if len(intensity)!=1 or len(brightness)!=1:
        raise RuntimeError('Estrutura da sequencia mudou; esperado uma faixa Intensity e uma MPC_Luz.')
    backup=ROOT/'Saved/Backup_Piscada_4s'/datetime.datetime.now().strftime('%Y%m%d_%H%M%S')
    backup.mkdir(parents=True)
    seqpath=seq.get_path_name().split('.')[0]
    if not seqpath.startswith('/Game/'):raise RuntimeError('Sequencia fora do projeto.')
    seqfile=ROOT/'Content'/(seqpath[6:]+'.uasset')
    shutil.copy2(seqfile,backup/seqfile.name)
    shutil.copy2(ROOT/'Content/Cenas/Cena_Menu.umap',backup/'Cena_Menu.umap')
    report['backup']=str(backup);report['sequence']=seq.get_path_name()
    section,channel=intensity[0]
    oldkeys=list(channel.get_keys());peak=max([float(k.get_value()) for k in oldkeys]+[0.0]) or 3150.0
    report['previous_key_count']=len(oldkeys)
    seq.modify();seq.set_display_rate(u.FrameRate(24,1));seq.set_playback_start(0);seq.set_playback_end(96)
    for key in oldkeys:channel.remove_key(key)
    section.set_range(0,96)
    for frame,value in [(0,peak),(90,0),(94,peak)]:
        channel.add_key(u.FrameNumber(frame),value,interpolation=u.MovieSceneKeyInterpolation.CONSTANT)
    ms=brightness[0];ms.remove_scalar_parameter('Brilho');ms.set_range(0,96)
    for frame,value in [(0,1.0),(90,0.0),(94,1.0)]:
        ms.add_scalar_parameter_key('Brilho',u.FrameNumber(frame),value,default_interpolation=u.MovieSceneKeyInterpolation.CONSTANT)
    if not u.EditorAssetLibrary.save_loaded_asset(seq):raise RuntimeError('Falha salvando sequencia.')
    actor.modify();settings=actor.get_editor_property('playback_settings')
    settings.set_editor_property('auto_play',True)
    settings.set_editor_property('play_rate',1.0)
    settings.set_editor_property('loop_count',u.MovieSceneSequenceLoopCount(-1))
    actor.set_editor_property('playback_settings',settings)
    if not level.save_current_level():raise RuntimeError('Falha salvando mapa.')
    report.update(complete=True,period_seconds=4.0,dark_seconds=4/24,peak_intensity=peak,frames=[0,90,94])
try:run()
except:report['error']=traceback.format_exc();u.log_error(report['error'])
(ROOT/'Saved/Piscada_4s_Aplicada.json').write_text(json.dumps(report,indent=2),encoding='utf8')
