#include "ProbeArena.h"
#include "Camera/CameraActor.h"
#include "Components/WidgetComponent.h"
#include "Engine/Canvas.h"
#include "Styling/SlateBrush.h"
#include "Widgets/Images/SImage.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Engine/Engine.h"
#include "Engine/Engine.h"
#include "Engine/Texture2D.h"
#include "GameFramework/PlayerController.h"
#include "ImageUtils.h"
#include "InputCoreTypes.h"
#include "Misc/Paths.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"

namespace {
using rpg::presentation::Rect;
struct TicketStyle { FColor Accent; double ArtTop; };
const TicketStyle& StyleFor(rpg::presentation::TerminalRoute route)
{
    static const TicketStyle styles[]={
        {FColor(191,104,239),650},
        {FColor(81,155,247),1030},
        {FColor(61,199,154),1410},
        {FColor(239,153,63),1795},
        {FColor(225,105,112),2180},
    };
    return styles[static_cast<std::size_t>(route)];
}
}

void AProbeArenaHUD::ToggleTerminalShortcut()
{
    if(!TerminalShortcutOpen){TerminalShortcutFirstTicket=TerminalFirstTicket;}
    TerminalShortcutOpen=!TerminalShortcutOpen;
    TerminalShortcutReturned=!TerminalShortcutOpen;
    HideTerminalWorldDock();
    Pointer.cancel_until_release();
}

void AProbeArenaHUD::DrawTerminalTickets(const Rect& safe, bool worldDock, bool focused3D)
{
    if(!TerminalArtChecked){
        TerminalArtChecked=true;
        const auto path=FPaths::ProjectContentDir()/TEXT("Presentation/assets/runtime/hub/terminal-multimodal-2d.png");
        TerminalArt=FImageUtils::ImportFileAsTexture2D(path);
        const auto folder=FPaths::ProjectContentDir()/TEXT("Presentation/assets/runtime/hub/terminal-neon");
        for(const TCHAR* file:{TEXT("title-face.png"),TEXT("title-halo.png"),TEXT("subtitle-face.png"),TEXT("subtitle-halo.png")}){
            TerminalSignTextures.Add(FImageUtils::ImportFileAsTexture2D(folder/file));
        }
    }
    const auto box=[&](const Rect& rect,FLinearColor color){
        DrawRect(color,rect.x,rect.y,rect.width,rect.height);
    };
    const auto text=[&](const FString& label,const Rect& rect,FColor color,float maxScale=1.f){
        float w=0,h=0;GetTextSize(label,w,h,HubBodyFont);
        const auto scale=FMath::Min3(maxScale,float(rect.width)/FMath::Max(w,1.f),float(rect.height)/FMath::Max(h,1.f));
        DrawText(label,color,rect.x,rect.y,HubBodyFont,scale);
    };
    const auto ticketName=[&](const rpg::presentation::TerminalTicket& ticket,FColor accent,const Rect& bounds){
        const FString label=FString(UTF8_TO_TCHAR(ticket.title)).ToUpper();
        UFont* font=GEngine?GEngine->GetMediumFont():nullptr;
        if(!font){font=HubBodyFont;}
        float w=0,h=0;
        GetTextSize(label,w,h,font);
        const float scale=FMath::Min(float(bounds.width)/FMath::Max(w,1.f),float(bounds.height)/FMath::Max(h,1.f));
        DrawText(label,accent,bounds.x+(bounds.width-w*scale)*0.5,bounds.y,font,scale);
    };
    const auto neonHeader=[&](const Rect& titleRect,const Rect& subtitleRect){
        const double phase=FMath::Fmod(FPlatformTime::Seconds(),5.8);
        const float flicker=HubReducedMotion?1.f:1.f-0.64f*
            FMath::Max(0.f,1.f-float(FMath::Abs(phase-5.20)/0.12));
        const auto drawSign=[&](int32 index,const TCHAR* label,const Rect& rect,FLinearColor haloColor){
            auto* face=TerminalSignTextures.IsValidIndex(index)?TerminalSignTextures[index].Get():nullptr;
            if(!face){
                float w=0,h=0;
                GetTextSize(label,w,h,HubBodyFont);
                const float scale=FMath::Min3(1.f,float(rect.width)/FMath::Max(w,1.f),float(rect.height)/FMath::Max(h,1.f));
                DrawText(label,FColor::White,rect.x+(rect.width-w*scale)*0.5,rect.y+(rect.height-h*scale)*0.5,HubBodyFont,scale);
                return;
            }
            constexpr double maskPadding=48;
            const double scale=FMath::Min(rect.width/(face->GetSizeX()-maskPadding),rect.height/(face->GetSizeY()-maskPadding));
            const double w=face->GetSizeX()*scale,h=face->GetSizeY()*scale;
            const double x=rect.x+(rect.width-w)*0.5,y=rect.y+(rect.height-h)*0.5;
            if(auto* halo=TerminalSignTextures[index+1].Get()){
                haloColor.A=0.78f*flicker;
                DrawTexture(halo,x,y,w,h,0,0,1,1,haloColor,BLEND_Translucent);
            }
            DrawTexture(face,x,y,w,h,0,0,1,1,FLinearColor::White,BLEND_Translucent);
        };
        drawSign(0,TEXT("TERMINAL URBANO"),titleRect,FLinearColor(0.43f,0.18f,1.f));
        drawSign(2,TEXT("Destinos e Modos Internos"),subtitleRect,FLinearColor(0.05f,0.8f,0.92f));
    };
    const auto button=[&](const FString& label,const Rect& rect,bool enabled=true){
        const bool hover=enabled && InputWindowActive && rpg::presentation::contains(rect,Input.x,Input.y);
        box(rect,hover?FLinearColor(0.12f,0.32f,0.42f):FLinearColor(0.06f,0.12f,0.22f));
        text(label,{rect.x+6,rect.y+12,rect.width-12,24},enabled?FColor::White:FColor(168,186,210),0.84f);
        return enabled && rpg::presentation::pointer_clicked(Input,rect);
    };
    const auto unavailable=[&](const Rect& action,const TCHAR* label=TEXT("Em breve")){
        box(action,FLinearColor(0.025f,0.04f,0.075f));
        float w=0,h=0;
        GetTextSize(label,w,h,HubBodyFont);
        constexpr double iconAndGap=19;
        const double scale=FMath::Clamp((action.width-8)/(w+iconAndGap),0.0,0.84);
        const double left=action.x+(action.width-(w+iconAndGap)*scale)*0.5;
        const double cy=action.y+action.height*0.5;
        const double cx=left+7*scale;
        const double radius=7*scale;
        const FLinearColor red(1.f,0.24f,0.28f);
        constexpr int32 segments=16;
        for(int32 i=0;i<segments;++i){
            const double start=2*PI*i/segments;
            const double end=2*PI*(i+1)/segments;
            DrawLine(cx+radius*FMath::Cos(start),cy+radius*FMath::Sin(start),
                cx+radius*FMath::Cos(end),cy+radius*FMath::Sin(end),red,1.5f);
        }
        const double slash=radius*0.7;
        DrawLine(cx-slash,cy+slash,cx+slash,cy-slash,red,1.5f);
        DrawText(label,FColor(214,224,239),left+iconAndGap*scale,cy-h*scale*0.5,HubBodyFont,scale);
    };
    const auto embark=[&](rpg::presentation::TerminalRoute route){
        if(!TerminalNav.embark(route)){return false;}
        Pointer.cancel_until_release();
        return true;
    };
    const auto close=[&](){
        if(TerminalShortcutOpen){ToggleTerminalShortcut();return;}
        Flow.back(false,false);
        Pointer.cancel_until_release();
    };
    // QA fixtures are local to this view. The catalog still controls embarkation.
    FString qaScenario;
    const bool qa=!worldDock && FParse::Value(FCommandLine::Get(),TEXT("TerminalQA="),qaScenario);
    const bool qaBalances=qa && qaScenario.Equals(TEXT("Saldo"),ESearchCase::IgnoreCase);
    const auto qaHeader=[&](const Rect& bounds){
        box(bounds,FLinearColor(0.025f,0.07f,0.13f,0.94f));
        if(qaBalances){
            text(TEXT("QA • Ouro 1.250  |  Gemas 32  |  Notas musicais 7"),
                {bounds.x+6,bounds.y+4,bounds.width-12,bounds.height-8},FColor(255,221,136),1.f);
        }else{
            text(TEXT("QA • Terminal 2D"),{bounds.x+6,bounds.y+4,bounds.width-12,bounds.height-8},FColor(255,221,136),1.f);
        }
    };
    const auto qaNote=[&](rpg::presentation::TerminalRoute route,const Rect& bounds){
        if(!qaBalances){return;}
        using rpg::presentation::TerminalRoute;
        if(route==TerminalRoute::Metro){
            text(TEXT("QA • Requer 20 notas (7 disponíveis)"),bounds,FColor(255,221,136),1.f);
        }else if(route==TerminalRoute::Bus){
            text(TEXT("QA • Bônus ilustrativo"),bounds,FColor(136,226,214),1.f);
        }
    };
    const auto tabs=[&](const Rect& dock){
        if(worldDock){
            DrawTerminalWorldDock(dock);
            if(rpg::presentation::pointer_clicked(Input,{dock.x,dock.y,dock.width*0.13,dock.height})){close();HideTerminalWorldDock();}
            return;
        }
        box(dock,FLinearColor(0.025f,0.05f,0.10f,0.88f));
        box({dock.x,dock.y,dock.width,2},FLinearColor(0.23f,0.43f,0.62f));
        const double backWidth=dock.width*0.13;
        const double portraitWidth=FMath::Min(48.0,dock.width*0.15);
        const double tabWidth=(dock.width-backWidth-portraitWidth-10)/3;
        if(button(TerminalShortcutOpen?TEXT("3D"):TEXT("‹"),{dock.x+2,dock.y+3,backWidth-4,38})){close();return;}
        const double portraitX=dock.x+backWidth;
        const double portraitSize=FMath::Min(portraitWidth-4,38.0);
        box({portraitX,dock.y+3,portraitWidth,38},FLinearColor(0.06f,0.26f,0.32f));
        if(auto* portrait=UiTexture(TEXT("avatar-felipe-v01"))){
            DrawTexture(portrait,portraitX+(portraitWidth-portraitSize)*0.5,dock.y+(44-portraitSize)*0.5,
                portraitSize,portraitSize,0,0,1,1,FLinearColor::White,BLEND_Translucent);
        }
        const double routesX=portraitX+portraitWidth+2;
        box({routesX,dock.y+3,tabWidth,38},FLinearColor(0.06f,0.27f,0.33f));
        box({routesX,dock.y+3,tabWidth,2},FLinearColor(0.14f,0.88f,0.91f));
        text(TEXT("ROTAS"),{routesX+6,dock.y+13,tabWidth-12,21},FColor(112,255,247),0.9f);
        unavailable({routesX+tabWidth+2,dock.y+3,tabWidth,38},TEXT("TRILHAS"));
        unavailable({routesX+2*(tabWidth+2),dock.y+3,tabWidth,38},TEXT("EXPEDIÇÃO"));
    };
    if(!worldDock){HideTerminalWorldDock();}
    const auto canvasSize=FVector2D(Canvas->SizeX,Canvas->SizeY);
    if(!worldDock){box({0,0,canvasSize.X,canvasSize.Y},FLinearColor(0.007f,0.014f,0.041f));}
    const bool mobileView=Preferences.mode==rpg::presentation::PresentationMode::Mobile ||
        (Preferences.mode==rpg::presentation::PresentationMode::Auto && Canvas->SizeY>Canvas->SizeX);
#if PLATFORM_ANDROID || PLATFORM_IOS
    constexpr bool mobilePlatform=true;
#else
    constexpr bool mobilePlatform=false;
#endif
    auto& firstTicket=TerminalShortcutOpen?TerminalShortcutFirstTicket:TerminalFirstTicket;
    const bool focused=HubView==rpg::presentation::HubView::Focused || mobileView || mobilePlatform;
    if(TerminalArt && focused && !worldDock){
        // Crop out the mock balances and the baked navigation bar. Place real controls in the art's coordinate space.
        constexpr double artLeft=165,artTop=185,artWidth=1675,artHeight=2545;
        const double scale=FMath::Min3((safe.width-12)/artWidth,(safe.height-56)/artHeight,0.52);
        const Rect scene{safe.x+(safe.width-artWidth*scale)*0.5,safe.y,artWidth*scale,artHeight*scale};
        const auto artX=[&](double value){return scene.x+(value-artLeft)*scale;};
        const auto artY=[&](double value){return scene.y+(value-artTop)*scale;};
        DrawTexture(TerminalArt,scene.x,scene.y,scene.width,scene.height,artLeft/2000,artTop/3000,
            artWidth/2000,artHeight/3000,FLinearColor::White,BLEND_Opaque);
        neonHeader({scene.x+12,artY(258),scene.width-24,33},{scene.x+12,artY(438),scene.width-24,19});
        if(qa){qaHeader({scene.x+12,artY(505),scene.width-24,FMath::Min(26.0,90*scale)});}
        const double actionHeight=FMath::Max(44.0,154*scale);
        for(const auto& ticket:rpg::presentation::terminal_tickets()){
            const auto& style=StyleFor(ticket.route);
            const double center=style.ArtTop+135;
            const double nameWidth=670*scale;
            const double ticketHeight=330*scale;
            if(!ticket.available){box({artX(294),artY(style.ArtTop-35),1418*scale,335*scale},FLinearColor(0.f,0.f,0.f,0.38f));}
            ticketName(ticket,style.Accent,{artX(660),artY(center-130),nameWidth,ticketHeight*0.23});
            text(UTF8_TO_TCHAR(ticket.subtitle),{artX(660),artY(center-24),nameWidth,ticketHeight*0.18},FColor(195,212,233),2.f);
            qaNote(ticket.route,{artX(660),artY(center+45),nameWidth,ticketHeight*0.17});
            const Rect action{artX(1410),artY(center)-actionHeight/2,274*scale,actionHeight};
            // Mask the entire baked action label, including the arrows, before drawing the real state.
            box({artX(1398),artY(center-86),296*scale,172*scale},FLinearColor(0.04f,0.075f,0.14f));
            if(!ticket.available){unavailable(action);}
            else if(button(TEXT("Embarcar"),action) && embark(ticket.route)){return;}
        }
        tabs({scene.x,scene.y+scene.height+4,scene.width,44});
        return;
    }

    const auto& tickets=rpg::presentation::terminal_tickets();
    const int32 ticketCount=static_cast<int32>(tickets.size());
    constexpr double gap=8;
    const double viewWidth=worldDock && focused3D?FMath::Min(canvasSize.X,canvasSize.Y*9.0/16.0):canvasSize.X;
    const double viewHeight=worldDock && focused3D?viewWidth*16.0/9.0:canvasSize.Y;
    const double viewY=(canvasSize.Y-viewHeight)*0.5;
    const double width=FMath::Min(680.0,FMath::Min(safe.width,viewWidth)-20);
    const double x=safe.x+(safe.width-width)*0.5;
    const double controlsY=FMath::Max(safe.y,viewY)+8;
    const double top=controlsY+(worldDock?50:2);
    if(worldDock){
        using rpg::presentation::HubArea;
        const HubArea sectors[]={HubArea::Left,HubArea::Center,HubArea::Right};
        const TCHAR* labels[]={TEXT("Esquerda"),TEXT("Centro"),TEXT("Direita")};
        const double cell=(width-12)/4;
        for(int32 index=0;index<3;++index){
            const Rect control{x+index*(cell+4),controlsY,cell,44};
            if(button(labels[index],control)){
                TerminalSector=sectors[index];
                TerminalShortcutReturned=false;
                Pointer.cancel_until_release();
                return;
            }
            if(TerminalSector==sectors[index]){
                box({control.x,control.y,control.width,2},FLinearColor(0.14f,0.88f,0.91f));
            }
        }
        const Rect shortcut{x+3*(cell+4),controlsY,cell,44};
        if(button(TEXT("2D / F2"),shortcut)){ToggleTerminalShortcut();return;}
        if(TerminalShortcutReturned){
            box({shortcut.x,shortcut.y,shortcut.width,2},FLinearColor(0.14f,0.88f,0.91f));
        }
    }
    const double footer=FMath::Min(safe.y+safe.height,viewY+viewHeight)-52;
    const double listTop=top+(qa?94:64);
    const double listHeight=footer-listTop-54;
    const double rowHeight=FMath::Clamp(listHeight/ticketCount-gap,56.0,96.0);
    const int32 count=FMath::Clamp(FMath::FloorToInt((listHeight+gap)/(rowHeight+gap)),1,ticketCount);
    const int32 maxFirst=ticketCount-count;
    const Rect list{x,listTop,width,listHeight};
    int32 scroll=0;
    if(rpg::presentation::contains(list,Input.x,Input.y)){
        scroll=int32(PlayerOwner->WasInputKeyJustPressed(EKeys::MouseScrollDown))-
               int32(PlayerOwner->WasInputKeyJustPressed(EKeys::MouseScrollUp));
    }
    const double dy=Input.y-Input.origin_y;
    if(Input.released && !Input.cancelled && rpg::presentation::contains(list,Input.origin_x,Input.origin_y)
        && FMath::Abs(dy)>=30 && FMath::Abs(dy)>FMath::Abs(Input.x-Input.origin_x)){
        scroll=dy<0?1:-1;
    }
    firstTicket=FMath::Clamp(firstTicket+scroll,0,maxFirst);


    box({x,top,width,4},FLinearColor(0.08f,0.35f,0.49f));
    neonHeader({x+12,top+8,width-24,30},{x+12,top+41,width-24,17});
    if(qa){qaHeader({x,top+62,width,26});}
    for(int32 offset=0;offset<count;++offset){
        const auto& ticket=tickets[firstTicket+offset];
        const auto& style=StyleFor(ticket.route);
        const double y=listTop+offset*(rowHeight+gap);
        const double artSize=FMath::Min(rowHeight-12,58.0);
        const double actionWidth=FMath::Clamp(width*0.27,76.0,138.0);
        const double labelX=x+artSize+18;
        const double labelWidth=width-(labelX-x)-actionWidth-12;
        const FLinearColor accent(style.Accent);
        const Rect row{x,y,width,rowHeight};
        box(row,accent*0.2f+FLinearColor(0.015f,0.022f,0.045f,0.8f));
        box({x,y,width,2},accent);
        if(TerminalArt){
            DrawTexture(TerminalArt,x+7,y+(rowHeight-artSize)*0.5,artSize,artSize,
                318.0/2000,style.ArtTop/3000,322.0/2000,280.0/3000,FLinearColor::White,BLEND_Opaque);
        }else{
            box({x+7,y+8,artSize,rowHeight-16},accent);
        }
        if(!ticket.available){box(row,FLinearColor(0.f,0.f,0.f,0.38f));}
        ticketName(ticket,style.Accent,{labelX,y+rowHeight*0.12,labelWidth,rowHeight*0.23});
        text(UTF8_TO_TCHAR(ticket.subtitle),{labelX,y+rowHeight*0.44,labelWidth,rowHeight*0.18},FColor(183,202,223),2.f);
        qaNote(ticket.route,{labelX,y+rowHeight*0.68,labelWidth,rowHeight*0.20});
        const Rect action{x+width-actionWidth-7,y+(rowHeight-44)*0.5,actionWidth,44};
        DrawLine(action.x-5,y+6,action.x-5,y+rowHeight-6,accent,1.0f);
        if(!ticket.available){unavailable(action);}
        else if(button(TEXT("Embarcar"),action) && embark(ticket.route)){return;}
    }
    if(maxFirst>0){
        if(button(TEXT("Anterior"),{x,footer-48,width*0.32,44},firstTicket>0)){--firstTicket;}
        if(button(TEXT("Mais"),{x+width*0.68,footer-48,width*0.32,44},firstTicket<maxFirst)){++firstTicket;}
    }
    tabs({x,footer,width,44});
}

void AProbeArenaHUD::HideTerminalWorldDock()
{
    if(TerminalWorldDock){TerminalWorldDock->SetVisibility(false);}
}

void AProbeArenaHUD::DrawTerminalWorldDock(const Rect& dock)
{
    constexpr int32 drawWidth=600;
    if(!TerminalWorldDock){
        TerminalWorldDock=NewObject<UWidgetComponent>(this);
        AddInstanceComponent(TerminalWorldDock);
        TerminalWorldDock->SetWidgetSpace(EWidgetSpace::World);
        TerminalWorldDock->SetBlendMode(EWidgetBlendMode::Transparent);
        TerminalWorldDock->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        TerminalWorldDock->SetTwoSided(true);
        TerminalWorldDock->SetBackgroundColor(FLinearColor::Transparent);
        TerminalWorldDock->RegisterComponent();
        TerminalPortraitBrush=MakeShared<FSlateBrush>();
        TerminalPortraitBrush->DrawAs=ESlateBrushDrawType::Image;
        TerminalPortraitBrush->SetResourceObject(UiTexture(TEXT("avatar-felipe-v01")));
        const auto brush=TerminalPortraitBrush;
        const auto tab=[](const TCHAR* label,FLinearColor fill,FLinearColor ink){
            return SNew(SBorder).Padding(3).BorderBackgroundColor(fill).HAlign(HAlign_Center).VAlign(VAlign_Center)
                [SNew(STextBlock).Text(FText::FromString(label)).ColorAndOpacity(FSlateColor(ink))];
        };
        TerminalWorldDock->SetSlateWidget(
            SNew(SBorder).Visibility(EVisibility::HitTestInvisible).Padding(3)
            .BorderBackgroundColor(FLinearColor(0.025f,0.05f,0.10f,0.88f))
            [
                SNew(SHorizontalBox)
                +SHorizontalBox::Slot().FillWidth(0.13f).Padding(1)
                [tab(TEXT("‹"),FLinearColor(0.08f,0.16f,0.25f),FLinearColor::White)]
                +SHorizontalBox::Slot().FillWidth(0.15f).Padding(1)
                [
                    SNew(SBorder).Padding(2).HAlign(HAlign_Center).VAlign(VAlign_Center)
                    .BorderBackgroundColor(FLinearColor(0.04f,0.28f,0.33f))
                    [SNew(SBox).WidthOverride(34).HeightOverride(34)
                        [SNew(SImage).Image_Lambda([brush](){return brush.Get();})]]
                ]
                +SHorizontalBox::Slot().FillWidth(0.24f).Padding(1)
                [tab(TEXT("ROTAS"),FLinearColor(0.06f,0.27f,0.33f),FLinearColor(0.40f,1.f,0.96f))]
                +SHorizontalBox::Slot().FillWidth(0.24f).Padding(1)
                [tab(TEXT("⊘  TRILHAS"),FLinearColor(0.025f,0.04f,0.075f),FLinearColor(0.70f,0.77f,0.85f))]
                +SHorizontalBox::Slot().FillWidth(0.24f).Padding(1)
                [tab(TEXT("⊘  EXPEDIÇÃO"),FLinearColor(0.025f,0.04f,0.075f),FLinearColor(0.70f,0.77f,0.85f))]
            ]);
    }

    const double y=dock.y+dock.height*0.5;
    const auto forward=Hub3DCamera->GetActorForwardVector();
    const auto cameraPosition=Hub3DCamera->GetActorLocation();
    constexpr double depth=40;
    const auto pointOnPlane=[&](double x,FVector& point){
        FVector origin,direction;
        if(!PlayerOwner->DeprojectScreenPositionToWorld(x,y,origin,direction)){return false;}
        const double alignment=FVector::DotProduct(direction,forward);
        if(alignment<=UE_SMALL_NUMBER){return false;}
        const double distance=(depth-FVector::DotProduct(origin-cameraPosition,forward))/alignment;
        point=origin+direction*distance;
        return true;
    };
    FVector left,right;
    if(dock.width<=0 || !pointOnPlane(dock.x,left) || !pointOnPlane(dock.x+dock.width,right)){
        HideTerminalWorldDock();
        return;
    }
    const double scale=FVector::Distance(left,right)/drawWidth;
    TerminalWorldDock->SetDrawSize(FVector2D(drawWidth,FMath::RoundToInt(drawWidth*dock.height/dock.width)));
    TerminalWorldDock->SetWorldLocationAndRotation((left+right)*0.5,
        Hub3DCamera->GetActorQuat()*FQuat(FVector::UpVector,UE_PI));
    TerminalWorldDock->SetWorldScale3D(FVector(scale));
    TerminalWorldDock->SetVisibility(true);
}