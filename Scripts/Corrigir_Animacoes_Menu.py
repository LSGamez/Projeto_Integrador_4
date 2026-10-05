"""Executar no Unreal: Tools > Execute Python Script. Mantem o mapa original do laboratorio."""
import unreal as u
import json, pathlib, shutil, datetime, traceback

ROOT=pathlib.Path(u.Paths.project_dir()).resolve()
OUT=ROOT/'Saved'/'Correcao_Animacoes_Menu'
OUT.mkdir(parents=True,exist_ok=True)
report={'complete':False}
def run():
    level=u.get_editor_subsystem(u.LevelEditorSubsystem)
    editor=u.get_editor_subsystem(u.UnrealEditorSubsystem)
    actorsys=u.get_editor_subsystem(u.EditorActorSubsystem)
    world=editor.get_editor_world()
    if world.get_path_name()!='/Game/Cenas/Cena_Menu.Cena_Menu':
        raise RuntimeError('Abra Content/Cenas/Cena_Menu, fora do Play, antes de executar.')
    if editor.get_game_world():
        raise RuntimeError('Pare o Play antes de executar.')
    actors=list(actorsys.get_all_level_actors())
    lights=[a for a in actors if a.get_actor_label()=='Luz_08' and isinstance(a,u.RectLight)]
    players=[a for a in actors if a.get_actor_label()=='Luz_Piscando' and isinstance(a,u.LevelSequenceActor)]
    caches=[a for a in actors if isinstance(a,u.GeometryCacheActor) and a.get_actor_label() in ('Frasco_01','Frasco_02','Frasco_05')]
    if len(lights)!=1 or len(players)!=1 or len(caches)!=3:
        raise RuntimeError('A cena mudou: esperado Luz_08, Luz_Piscando e os 3 frascos. Nenhuma alteracao aplicada.')
    oldseq=players[0].get_sequence()
    if not oldseq:raise RuntimeError('Luz_Piscando esta sem sequencia.')
    stamp=datetime.datetime.now().strftime('%Y%m%d_%H%M%S')
    backup=OUT/('Backup_'+stamp);backup.mkdir()
    mapfile=ROOT/'Content/Cenas/Cena_Menu.umap'
    if mapfile.exists():shutil.copy2(mapfile,backup/'Cena_Menu_antes_de_salvar.umap')
    if not level.save_current_level():raise RuntimeError('Nao foi possivel salvar a cena antes de corrigir.')
    shutil.copy2(mapfile,backup/'Cena_Menu.umap')
    report['backup']=str(backup)
    report['original_sequence']=oldseq.get_path_name()
    report['transforms_before']={a.get_actor_label():str(a.get_actor_transform()) for a in caches+lights}
    assettools=u.AssetToolsHelpers.get_asset_tools()
    name='Luz_Menu_'+stamp
    seq=assettools.create_asset(name,'/Game/Laboratorio/Animacoes',u.LevelSequence,u.LevelSequenceFactoryNew())
    if not seq:raise RuntimeError('Nao foi possivel criar a sequencia local.')
    seq.set_display_rate(u.FrameRate(24,1));seq.set_tick_resolution(u.FrameRate(24,1))
    seq.set_playback_start(0);seq.set_playback_end(96)
    light=lights[0];component=light.get_component_by_class(u.RectLightComponent)
    actorbinding=seq.add_possessable(light)
    binding=seq.add_possessable(component);binding.set_parent(actorbinding)
    track=binding.add_track(u.MovieSceneFloatTrack);track.set_property_name_and_path('Intensity','Intensity')
    section=track.add_section();section.set_range(0,97)
    channel=section.get_all_channels()[0]
    keys=[(1,1),(10,1),(11,.05),(12,.8),(14,.02),(17,1),(27,1),(28,0),(30,.35),(31,1),(45,1),(46,.08),(48,1),(59,1),(60,.02),(65,.45),(66,1),(79,1),(80,.03),(81,.7),(82,0),(86,1),(97,1)]
    for frame,value in keys:
        channel.add_key(u.FrameNumber(frame-1),3150*value,interpolation=u.MovieSceneKeyInterpolation.CONSTANT)
    mpc=u.load_asset('/Game/Laboratorio/Materiais/MPC_Luz')
    if not mpc:raise RuntimeError('MPC_Luz nao encontrado.')
    mt=seq.add_track(u.MovieSceneMaterialParameterCollectionTrack);mt.set_editor_property('mpc',mpc)
    ms=mt.add_section();ms.set_range(0,97)
    for frame,value in keys:
        ms.add_scalar_parameter_key('Brilho',u.FrameNumber(frame-1),value,default_interpolation=u.MovieSceneKeyInterpolation.CONSTANT)
    if not u.EditorAssetLibrary.save_loaded_asset(seq):raise RuntimeError('Falha salvando a sequencia.')
    with u.ScopedEditorTransaction('Corrigir animacoes do laboratorio no menu'):
        player=players[0];player.modify();player.set_sequence(seq)
        settings=player.get_editor_property('playback_settings')
        settings.set_editor_property('auto_play',True)
        settings.set_editor_property('loop_count',u.MovieSceneSequenceLoopCount(-1))
        player.set_editor_property('playback_settings',settings)
        component.modify();component.set_mobility(u.ComponentMobility.MOVABLE)
        report['frascos']=[]
        for a in caches:
            c=a.get_geometry_cache_component();c.modify()
            c.set_looping(True);c.set_editor_property('running',True)
            c.set_editor_property('auto_activate',True);c.set_editor_property('manual_tick',False)
            c.set_playback_speed(1.0);c.set_component_tick_enabled(True)
            c.set_mobility(u.ComponentMobility.MOVABLE)
            report['frascos'].append({'name':a.get_actor_label(),'cache':c.get_editor_property('geometry_cache').get_path_name(),'running':c.get_editor_property('running'),'looping':c.get_editor_property('looping')})
        if not level.save_current_level():raise RuntimeError('Falha salvando Cena_Menu.')
    report['sequence']=seq.get_path_name()
    report['light']=light.get_path_name()
    report['complete']=True
    u.log('ANIMACOES_MENU_CORRIGIDAS: '+seq.get_path_name())
try:
    run()
except:
    report['error']=traceback.format_exc()
    u.log_error(report['error'])
finally:
    (OUT/'Resultado.json').write_text(json.dumps(report,ensure_ascii=False,indent=2),encoding='utf8')
